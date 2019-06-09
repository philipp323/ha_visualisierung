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
        lastClick = intersectObjects[0].point;

        if(selectedRoom != undefined){
            selectedRoom.material.color = white;
        }

        selectedRoom = intersectObjects[0].object;
        randomColor = new THREE.Color(Math.random() * 0xffffff);
        selectedRoom.material.color = randomColor;

        ANNOTATION.style.display = 'initial';
        updateAnnotation(selectedRoom.name, "");

        


        /*if (MODE == "INFOPOINT_CREATION") {
            $('#infopointVerificationModal').modal("show");
            var infopoint = YOUR_CLICKABLE_INFOPOINTS[YOUR_CLICKABLE_INFOPOINTS.length - 1];
            infopoint.position.x = posX;
            infopoint.position.y = posY;
            infopoint.position.z = posZ;
            scene.add(infopoint);

            MODE = "NORMAL";
            $('#content').off('mousedown', onDocumentMouseDown);
        }*/
    }
}