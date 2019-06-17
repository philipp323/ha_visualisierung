function filterVisableRooms(){
    var helpChar;
    if (floors[0].toString() == visableFloorName) {
      helpChar = "U";
    }
    if (floors[1].toString() == visableFloorName) {
      helpChar = "E";
    }
    if (floors[2].toString() == visableFloorName) {
      helpChar = "1";
    }
    if (floors[3].toString() == visableFloorName) {
      helpChar = "2";
    }
    visableRooms = objectArr.filter(
      //filter visable(clickable)Rooms, Floors shouldn't be there.
      x =>
        x.name[0] == helpChar &&
        x.name != floors[0] &&
        x.name != floors[1] &&
        x.name != floors[2] &&
        x.name != floors[3]
    );
}

function handleEvents(){
    intersectObjects = raycaster.intersectObjects(visableRooms, true);
    intersectedFloors = raycaster.intersectObjects(objects, true);

    if (intersectObjects.length > 0 && MODE == "ROOM") {
        latestMouseIntersection = intersectObjects[0].point;

        hideTooltip();

        selectedRoom = intersectObjects[0].object;
        randomColor = new THREE.Color(Math.random() * 0xffffff);
        selectedRoom.material.color = randomColor;

        showTooltip();
        updateTooltip(selectedRoom.name, "23", "2000", "offen")
    }
    if (intersectedFloors.length > 0 && MODE == "FLOOR"){
        floorName = intersectedFloors[0].object.name;
        if(floors.includes(floorName)){
            MODE = "ROOM";
            floorSelect(floorName);
        }
    }
}

async function onDocumentMouseDown(event) {
    event.preventDefault();

    mouse3D = new THREE.Vector3(
        ((event.clientX - renderer.domElement.offsetLeft) / window.innerWidth) * 2 - 1,
        -((event.clientY - renderer.domElement.offsetTop) / window.innerHeight) * 2 + 1,
        0.5);
    raycaster.setFromCamera(mouse3D, camera);

    filterVisableRooms();

    //console.log(visableRooms);

    console.log("Entered Mouse-Down");
    handleEvents();
}

async function onDocumentTouchDown(event) {
    mouse3D = new THREE.Vector3(
        ((event.changedTouches[0].clientX - renderer.domElement.offsetLeft) /
        window.innerWidth) *2 - 1, -
        ((event.changedTouches[0].clientY - renderer.domElement.offsetTop) /
        window.innerHeight) *2 + 1, 0.5
    );
    raycaster.setFromCamera(mouse3D, camera);

    filterVisableRooms();

    console.log("Entered Touch-Down");
    handleEvents();
}