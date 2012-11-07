#include "MTTStudies/Geometry/interface/MTTSensitiveDetector.h"

#include "DataFormats/GeometryVector/interface/LocalPoint.h"
#include "DataFormats/GeometryVector/interface/LocalVector.h"

#include "SimG4Core/Notification/interface/TrackInformation.h"
#include "SimG4Core/Notification/interface/G4TrackToParticleID.h"
#include "SimG4Core/Physics/interface/G4ProcessTypeEnumerator.h"

#include "SimDataFormats/TrackingHit/interface/UpdatablePSimHit.h"
#include "SimDataFormats/SimHitMaker/interface/TrackingSlaveSD.h"
 
#include "SimG4Core/Notification/interface/TrackInformation.h"

#include "FWCore/Framework/interface/ESTransientHandle.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "Geometry/MuonNumbering/interface/MuonSubDetector.h"
#include "Geometry/MuonNumbering/interface/MuonBaseNumber.h"
#include "MTTStudies/Geometry/interface/MTTNumberingScheme.h"

#include "G4Track.hh"
#include "G4SDManager.hh"
#include "G4VProcess.hh"
#include "G4EventManager.hh"
#include "G4Event.hh"
#include "G4VProcess.hh"

#include <string>
#include <vector>
#include <iostream>


MTTSensitiveDetector::MTTSensitiveDetector(std::string name, 
					   const DDCompactView & cpv,
					   SensitiveDetectorCatalog & clg, 
					   edm::ParameterSet const & p,
					   const SimTrackManager* manager) : 
  SensitiveTkDetector(name, cpv, clg, p), myName(name), mySimHit(0),
  oldVolume(0), lastId(0), lastTrack(0), eventno(0) {
  
  edm::ParameterSet m_TrackerSD = p.getParameter<edm::ParameterSet>("MTTSD");
  energyCut           = m_TrackerSD.getParameter<double>("EnergyThresholdForPersistencyInGeV")*GeV; //default must be 0.5
  energyHistoryCut    = m_TrackerSD.getParameter<double>("EnergyThresholdForHistoryInGeV")*GeV;//default must be 0.05

  edm::LogInfo("MTTSensitiveDetector") <<"Criteria for Saving Tracker SimTracks: \n "
				       <<" History: "<<energyHistoryCut<< " MeV ; Persistency: "<< energyCut<<" MeV\n"
				       <<" Constructing a MTTSensitiveDetector with ";

  slave  = new TrackingSlaveSD(name);
  
  // Now attach the right detectors (LogicalVolumes) to me
  std::vector<std::string>  lvNames = clg.logicalNames(name);
  this->Register();
  for (std::vector<std::string>::iterator it = lvNames.begin();  
       it != lvNames.end(); it++)  {
    edm::LogInfo("MTTSensitiveDetector")<< name << " attaching LV " << *it;
    this->AssignSD(*it);
  }

  theG4ProcessTypeEnumerator = new G4ProcessTypeEnumerator;
  myG4TrackToParticleID = new G4TrackToParticleID;

  //detector = new MuonSubDetector(name);

  numbering  = new MTTNumberingScheme(cpv);
  g4numbering = new MuonG4Numbering(cpv);
}

MTTSensitiveDetector::~MTTSensitiveDetector() { 
  delete slave;
  delete theG4ProcessTypeEnumerator;
  delete myG4TrackToParticleID;
  delete numbering;
  delete g4numbering;
}

bool MTTSensitiveDetector::ProcessHits(G4Step * aStep,  G4TouchableHistory *) {

  LogDebug("MTTSensitiveDetector") << " Entering a new Step " 
		    << aStep->GetTotalEnergyDeposit() << " " 
		    << aStep->GetPreStepPoint()->GetPhysicalVolume()->GetLogicalVolume()->GetName();

  G4Track * gTrack  = aStep->GetTrack(); 
  if ((unsigned int)(gTrack->GetTrackID()) != lastTrack) {
    
    if (gTrack->GetKineticEnergy() > energyCut){
      TrackInformation* info = getOrCreateTrackInformation(gTrack);
      info->storeTrack(true);
    }
    if (gTrack->GetKineticEnergy() > energyHistoryCut){
      TrackInformation* info = getOrCreateTrackInformation(gTrack);
      info->putInHistory();
    }
  }

  if (aStep->GetTotalEnergyDeposit()>0.) {
    if (newHit(aStep) == true) {
      sendHit();
      createHit(aStep);
    } else {
      updateHit(aStep);
    }
    return true;
  }
  return false;
}

uint32_t MTTSensitiveDetector::setDetUnitId(G4Step * aStep) {
	  MuonBaseNumber num = g4numbering->PhysicalVolumeToBaseNumber(aStep);
	  return numbering->baseNumberToUnitNumber(num);
  //unsigned int detId = 0;

  //LogDebug("MTTSensitiveDetector")<< " DetID = "<<detId;

  //return detId;
}

void MTTSensitiveDetector::EndOfEvent(G4HCofThisEvent *) {
  
  LogDebug("MTTSensitiveDetector")<< " Saving the last hit in a ROU " << myName;

  if (mySimHit == 0) return;
  sendHit();
}

void MTTSensitiveDetector::fillHits(edm::PSimHitContainer& c, std::string n){
  if (slave->name() == n)  c=slave->hits();
}

void MTTSensitiveDetector::sendHit() {  
  if (mySimHit == 0) return;
  LogDebug("MTTSensitiveDetector") << " Storing PSimHit: " << pname << " " << mySimHit->detUnitId() 
				   << " " << mySimHit->trackId() << " " << mySimHit->energyLoss() 
				   << " " << mySimHit->entryPoint() << " " << mySimHit->exitPoint();
    
  slave->processHits(*mySimHit); 

  // clean up
  delete mySimHit;
  mySimHit = 0;
  lastTrack = 0;
  lastId = 0;
}

void MTTSensitiveDetector::updateHit(G4Step * aStep) {

  Local3DPoint theExitPoint = SensitiveDetector::FinalStepPosition(aStep,LocalCoordinates); 
  float theEnergyLoss = aStep->GetTotalEnergyDeposit()/GeV;
  mySimHit->setExitPoint(theExitPoint);
  LogDebug("MTTSensitiveDetector")<< " Before : " << mySimHit->energyLoss();
  mySimHit->addEnergyLoss(theEnergyLoss);
  globalExitPoint = SensitiveDetector::FinalStepPosition(aStep,WorldCoordinates);

  LogDebug("MTTSensitiveDetector") << " Updating: new exitpoint " << pname << " " 
				   << theExitPoint << " new energy loss " << theEnergyLoss 
				   << "\n Updated PSimHit: " << mySimHit->detUnitId() 
				   << " " << mySimHit->trackId()
				   << " " << mySimHit->energyLoss() << " " 
				   << mySimHit->entryPoint() << " " << mySimHit->exitPoint();
}

Local3DPoint MTTSensitiveDetector::toOrcaUnits(Local3DPoint in){
  return Local3DPoint(in.x()/cm,in.y()/cm,in.z()/cm);
}

Global3DPoint MTTSensitiveDetector::toOrcaUnits(Global3DPoint in){
  return Global3DPoint(in.x()/cm,in.y()/cm,in.z()/cm);
}


bool MTTSensitiveDetector::newHit(G4Step * aStep) {

  G4Track * theTrack = aStep->GetTrack(); 
  uint32_t theDetUnitId = setDetUnitId(aStep);
  unsigned int theTrackID = theTrack->GetTrackID();

  LogDebug("MTTSensitiveDetector") << " OLD (d,t) = (" << lastId << "," << lastTrack 
				   << "), new = (" << theDetUnitId << "," << theTrackID << ") return "
				   << ((theTrackID == lastTrack) && (lastId == theDetUnitId));
  if ((mySimHit != 0) && (theTrackID == lastTrack) && (lastId == theDetUnitId) && closeHit(aStep))
    return false;
  return true;
}

bool MTTSensitiveDetector::closeHit(G4Step * aStep) {

  if (mySimHit == 0) return false; 
  const float tolerance = 0.05 * mm; // 50 micron are allowed between the exit 
  // point of the current hit and the entry point of the new hit
  Local3DPoint theEntryPoint = SensitiveDetector::InitialStepPosition(aStep,LocalCoordinates);  
  LogDebug("MTTSensitiveDetector")<< " closeHit: distance = " << (mySimHit->exitPoint()-theEntryPoint).mag();

  if ((mySimHit->exitPoint()-theEntryPoint).mag()<tolerance) return true;
  return false;
}

void MTTSensitiveDetector::createHit(G4Step * aStep) {

  if (mySimHit != 0) {
    delete mySimHit;
    mySimHit=0;
  }
    
  G4Track * theTrack  = aStep->GetTrack(); 
  G4VPhysicalVolume * v = aStep->GetPreStepPoint()->GetPhysicalVolume();

  Local3DPoint theEntryPoint = toOrcaUnits(SensitiveDetector::InitialStepPosition(aStep,LocalCoordinates));
  Local3DPoint theExitPoint  = toOrcaUnits(SensitiveDetector::FinalStepPosition(aStep,LocalCoordinates));
  
  float thePabs             = aStep->GetPreStepPoint()->GetMomentum().mag()/GeV;
  float theTof              = aStep->GetPreStepPoint()->GetGlobalTime()/nanosecond;
  float theEnergyLoss       = aStep->GetTotalEnergyDeposit()/GeV;
  int theParticleType       = myG4TrackToParticleID->particleID(theTrack);
  uint32_t theDetUnitId     = setDetUnitId(aStep);
  
  globalEntryPoint = SensitiveDetector::InitialStepPosition(aStep,WorldCoordinates);
  globalExitPoint = SensitiveDetector::FinalStepPosition(aStep,WorldCoordinates);
  pname = theTrack->GetDynamicParticle()->GetDefinition()->GetParticleName();
  
  unsigned int theTrackID = theTrack->GetTrackID();
  
  G4ThreeVector gmd  = aStep->GetPreStepPoint()->GetMomentumDirection();
  // convert it to local frame
  G4ThreeVector lmd = ((G4TouchableHistory *)(aStep->GetPreStepPoint()->GetTouchable()))->GetHistory()->GetTopTransform().TransformAxis(gmd);
  Local3DPoint lnmd = ConvertToLocal3DPoint(lmd);
  float theThetaAtEntry = lnmd.theta();
  float thePhiAtEntry = lnmd.phi();
    
  mySimHit = new UpdatablePSimHit(theEntryPoint,theExitPoint,thePabs,theTof,
				  theEnergyLoss,theParticleType,theDetUnitId,
				  theTrackID,theThetaAtEntry,thePhiAtEntry,
				  theG4ProcessTypeEnumerator->processId(theTrack->GetCreatorProcess()));  

  LogDebug("MTTSensitiveDetector") << " Created PSimHit: " << pname << " " 
				   << mySimHit->detUnitId() << " " << mySimHit->trackId()
				   << " " << mySimHit->energyLoss() << " " 
				   << mySimHit->entryPoint() << " " << mySimHit->exitPoint();
  lastId = theDetUnitId;
  lastTrack = theTrackID;
  oldVolume = v;
}

void MTTSensitiveDetector::update(const BeginOfJob * ) { }

void MTTSensitiveDetector::update(const BeginOfEvent * i) {

  clearHits();
  eventno = (*i)()->GetEventID();
  mySimHit = 0;
}

void MTTSensitiveDetector::update(const BeginOfTrack *bot) {

  const G4Track* gTrack = (*bot)();
  pname = gTrack->GetDynamicParticle()->GetDefinition()->GetParticleName();
}

void MTTSensitiveDetector::clearHits() {
    slave->Initialize();
}

TrackInformation* MTTSensitiveDetector::getOrCreateTrackInformation( const G4Track* gTrack) {
  G4VUserTrackInformation* temp = gTrack->GetUserInformation();
  if (temp == 0){
    edm::LogError("MTTSensitiveDetector") <<" ERROR: no G4VUserTrackInformation available";
    abort();
  }else{
    TrackInformation* info = dynamic_cast<TrackInformation*>(temp);
    if (info == 0){
      edm::LogError("MTTSensitiveDetector") <<" ERROR: TkSimTrackSelection: the UserInformation does not appear to be a TrackInformation";
      abort();
    }
    return info;
  }
}
