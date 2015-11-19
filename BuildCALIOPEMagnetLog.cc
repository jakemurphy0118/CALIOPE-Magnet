



G4LogicalVolume* DetectorConstruction::BuildCALIOPEMagnetLog(){
/*This function returns a logical volume of the magnet.  It can be placed in Construct() function 
by the following method:

G4LogicalVolume* magnet = BuildCALIOPEMagnetLog();
new G4PVPlacement(rotation, position, magnet, "Magnet", the worlds's logical volume, false, 0, true);

DetectorConstuction.hh should include the following:

G4LogicalVolume* BuildCALIOPEMagnetLog();

NOTE:
-the empty space in the magnet's volume is "G4Galactic" vacuum.  this probably needs to be adjusted
to whatever the world's logic empty space is
*/
	
	G4NistManager* man = G4NistManager::Instance();

//------------------------------create world box 
	G4double world_hx = 2.0*m;
	G4double world_hy = 2.0*m;
	G4double world_hz = 2.0*m;
	G4Box* worldBox = new G4Box("World", world_hx, world_hy, world_hz);
	
	G4Material *Vacuum = man->FindOrBuildMaterial("G4_Galactic");

	G4LogicalVolume* magnetLog = new G4LogicalVolume(worldBox, Vacuum, "World");


	//VARIABLES
	//origin is center of source position 
	G4double origx = 0.0;
	G4double origy = 0.0;
	G4double origz = 0.0;

	G4double radius = 58*mm;  //radius of all the iron bars (except at the poles)
	G4double poleRadius = 42.0*mm; //iron radius at the poles
	G4double chamferHeight1 = 18.75*mm; //height of the tapered iron bar section
	G4double chamferHeight2 = 18.75*mm; //height from top of copper coils to support corner box
	G4double gapHeight = 141.4*mm; //distance between the poles

	G4double height = 900*mm; //height from bottom of the top bar to top of the bottom bar 
	G4double width = 500*mm;  //I believe the width was defined to be the distance from the center of the source placement to the edge of the other bar :/
	G4double maxWireLayers = 15.0;
	G4double wireDiameter = 4.621*mm;
	G4double copperCylHalfz = 125*mm;
	

	//define materials
	G4double a;
	G4double density;
	G4double z;
	G4String name;

	a = 55.85*g/mole;
	G4Element* elFe = new G4Element("Iron" , "Fe" , z= 26., a);
	
	density = 7.87*g/cm3;
	G4Material* sFe = new G4Material(name="solidIron", z=26., a, density);

	a = 63.546*g/mole;
	density = 8.96*g/cm3;
	G4Material* sCu = new G4Material(name="solidCopper", z=29., a, density);


	//reference origin box
	//G4Box* refBox = new G4Box("RefBox", 1.0*mm, 1.0*mm, 1.0*mm);
	//G4LogicalVolume* refBoxLog = new G4LogicalVolume(refBox, sFe, "refBox");
	//new G4PVPlacement(0, G4ThreeVector(origx, origy, origz), refBoxLog, "REF", magnetLog, false, 0, true);

	//poles (cutoff pieces)
	G4double Pi = pi; 
	G4Cons* poleCone = new G4Cons("pole", 0.0, poleRadius, 0.0, radius, chamferHeight1/2, 0.0, 2*Pi);

	G4LogicalVolume* poleLog = new G4LogicalVolume(poleCone, sFe, "Pole");

	G4double pos_x = origx;
	G4double pos_y = origy;
	G4double pos_z = origz+(gapHeight/2)+(chamferHeight1/2);
	//G4VPhysicalVolume* fPhysBottomPole = 
	new G4PVPlacement(0, G4ThreeVector(pos_x, pos_y, pos_z), poleLog, "BottomPole", magnetLog, false, 0, true);
			
	G4RotationMatrix *rot1 = new G4RotationMatrix();
	rot1->rotateY(180*deg);
	pos_z = origz-(gapHeight/2)-(chamferHeight1/2);	
	//G4VPhysicalVolume* fPhysTopPole = 
	new G4PVPlacement(rot1, G4ThreeVector(pos_x, pos_y, pos_z), poleLog, "TopPole", magnetLog, false, 0, true);
	
	//iron bars (sourceBar is one with coils, longBar is the longest, supportBar is the top and bottom bar)
	
	G4double halfz1 = ((height/2)-(gapHeight/2)-chamferHeight1)/2;
	G4Tubs* sourceBar = new G4Tubs("souceBar", 0.0, radius, halfz1, 0.0, 2*Pi);
	G4double halfz2 = height/2;
	G4Tubs* longBar = new G4Tubs("longBar", 0.0, radius, halfz2, 0.0, 2*Pi);
	G4double halfz3 = (width-poleRadius)/2;
	G4Tubs* supportBar = new G4Tubs("supportBar", 0.0, radius, halfz3, 0.0, 2*Pi);

	G4LogicalVolume* sourceBarLog = new G4LogicalVolume(sourceBar, sFe, "sourceBar");
	G4LogicalVolume* longBarLog = new G4LogicalVolume(longBar, sFe, "longBar");
	G4LogicalVolume* supportBarLog = new G4LogicalVolume(supportBar, sFe, "supportBar");

	pos_z = origz + (gapHeight/2) + chamferHeight1 + halfz1;
	//G4VPhysicalVolume* fPhysTopSourceBar =
	new G4PVPlacement(0, G4ThreeVector(pos_x, pos_y, pos_z), sourceBarLog, "TopSourceBar", magnetLog, false, 0, true);
	pos_z = origz - (gapHeight/2) - chamferHeight1 - halfz1;
	//G4VPhysicalVolume* fPhysBottomSourceBar =
	new G4PVPlacement(0, G4ThreeVector(pos_x, pos_y, pos_z), sourceBarLog, "BottomSourceBar", magnetLog, false, 0, true);

	pos_x = origx - width - radius;
	pos_z = origz;
	//G4VPhysicalVolume* fLongBar = 
	new G4PVPlacement(0, G4ThreeVector(pos_x, pos_y, pos_z), longBarLog, "LongBar", magnetLog, false, 0, true);

	pos_x = origx - radius - halfz3;
	pos_z = origz + (height/2) + (radius);
	G4RotationMatrix* rot2 = new G4RotationMatrix();
	rot2->rotateY(90*deg);
	//G4VPhysicalVolume* fTopSupportBar = 
	new G4PVPlacement(rot2, G4ThreeVector(pos_x, pos_y, pos_z), supportBarLog, "TopSupportBar", magnetLog, false, 0, true);
	pos_z = origx - (height/2) - radius;
	//G4VPhysicalVolume* fBottomSupportBar = 
	new G4PVPlacement(rot2, G4ThreeVector(pos_x, pos_y, pos_z), supportBarLog, "BottomSupportBar", magnetLog, false, 0, true);

	//corner box supports (this might not be in the actual design?), boxes are numbered CCW from source around the frame
	G4Box* cornerBox = new G4Box("cornerBox", radius, radius, radius);

	G4LogicalVolume* cornerBoxLog = new G4LogicalVolume(cornerBox, sFe, "cornerBox");

	pos_z = origz + (height/2) + radius;
	pos_x = origx;
	//G4VPhysicalVolume* fCornerBox1 = 
	new G4PVPlacement(0, G4ThreeVector(pos_x, pos_y, pos_z), cornerBoxLog, "CornerBox1", magnetLog, false, 0, true);
	pos_z = -pos_z;
	//G4VPhysicalVolume* fCornerBox4 =
	new G4PVPlacement(0, G4ThreeVector(pos_x, pos_y, pos_z), cornerBoxLog, "CornerBox4", magnetLog, false, 0, true);
	pos_x = origx - width - radius;
	//G4VPhysicalVolume* fCornerBox3 =
	new G4PVPlacement(0, G4ThreeVector(pos_x, pos_y, pos_z), cornerBoxLog, "CornerBox3", magnetLog, false, 0, true);
	pos_z = -pos_z;
	//G4VPhysicalVolume* fCornerBox2 =
	new G4PVPlacement(0, G4ThreeVector(pos_x, pos_y, pos_z), cornerBoxLog, "CornerBox2", magnetLog, false, 0, true);

	//copper cylinders (just solid copper) 
	G4Tubs* copperCyl = new G4Tubs("copperCyl", radius, radius + (maxWireLayers*wireDiameter), copperCylHalfz, 0, 2*Pi);

	G4LogicalVolume* copperCylLog = new G4LogicalVolume(copperCyl, sCu, "copperCyl");

	pos_z = origz + (height/2) - chamferHeight2 - copperCylHalfz;
	pos_x = 0.0;
	//G4VPhysicalVolume* fTopCopperCyl =
	new G4PVPlacement(0, G4ThreeVector(pos_x, pos_y, pos_z), copperCylLog, "TopCopperCyl", magnetLog, false, 0, true);
	pos_z = -pos_z;
	//G4VPhysicalVolume* fBottomCopperCyl =
	new G4PVPlacement(0, G4ThreeVector(pos_x, pos_y, pos_z), copperCylLog, "BottomCopperCyl", magnetLog, false, 0, true);

	//copper cones
	G4double halfz4 = (((height/2)- (gapHeight/2) - chamferHeight1 - chamferHeight2 - (2*copperCylHalfz))/2);
	G4Cons* copperCone = new G4Cons("copperCone", radius, radius, radius, radius + (maxWireLayers*wireDiameter), halfz4, 0, 2*Pi);

	G4LogicalVolume* copperConeLog = new G4LogicalVolume(copperCone, sCu, "copperCone");

	pos_z = origz + (gapHeight/2) + chamferHeight1 + halfz4;
	//G4VPhysicalVolume* fTopCopperCone = 
	new G4PVPlacement(0, G4ThreeVector(pos_x, pos_y, pos_z), copperConeLog, "TopCopperCone", magnetLog, false, 0, true);
	pos_z = -pos_z;
	//G4VPhysicalVolume* fBottomCopperCone =
	new G4PVPlacement(rot1, G4ThreeVector(pos_x, pos_y, pos_z), copperConeLog, "BottomCopperCone", magnetLog, false, 0, true);

	//COLOURS :)
	G4VisAttributes* ironVisAtt = new G4VisAttributes(G4Colour(0.3,0.3,0.3));
	poleLog->SetVisAttributes(ironVisAtt);
	sourceBarLog->SetVisAttributes(ironVisAtt);
	longBarLog->SetVisAttributes(ironVisAtt);
	supportBarLog->SetVisAttributes(ironVisAtt);

	G4VisAttributes* cornerVisAtt = new G4VisAttributes(G4Colour(0.2,0.2,0.2));
	cornerBoxLog->SetVisAttributes(cornerVisAtt);

	G4VisAttributes* copperVisAtt = new G4VisAttributes(G4Colour(0.8,0.4,0.0));
	copperCylLog->SetVisAttributes(copperVisAtt);
	copperConeLog->SetVisAttributes(copperVisAtt);



	return magnetLog;
	
}
