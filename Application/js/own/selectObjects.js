async function onDocumentMouseDown(event) {

    event.preventDefault();

    mouse3D = new THREE.Vector3(
        ((event.clientX - renderer.domElement.offsetLeft) / window.innerWidth) * 2 - 1,
        -((event.clientY - renderer.domElement.offsetTop) / window.innerHeight) * 2 + 1,
        0.5);
    var raycaster = new THREE.Raycaster();
    raycaster.setFromCamera(mouse3D, camera);

    intersectObjects = raycaster.intersectObjects(roomObjects, true);
    //intersectInfopoints = raycaster.intersectObjects(YOUR_CLICKABLE_INFOPOINTS, true);
    console.log("Entered Mouse-Down");
    if (intersectObjects.length > 0) {
        latestMouseIntersection = intersectObjects[0].point;

        if(selectedRoom != undefined){
            selectedRoom.material.color = white;
        }

        selectedRoom = intersectObjects[0].object;
        randomColor = new THREE.Color(Math.random() * 0xffffff);
        selectedRoom.material.color = randomColor;

        showTooltip();
        updateTooltip(selectedRoom.name, "23", "2000", "offen")
    }
}