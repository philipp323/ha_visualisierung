function loadModel() {
var manager = new THREE.LoadingManager();
            manager.onStart = function (url, itemsLoaded, itemsTotal) {
                console.log(
                    "Started loading file: " +
                    url +
                    ".\nLoaded " +
                    itemsLoaded +
                    " of " +
                    itemsTotal +
                    " files."
                );
            };

            //STOCKWERKE FILTER (Vorgegeben)
            manager.onLoad = function () {
                roomObjects = objectArr.filter(
                    x =>
                    x.name[0] == "U" ||
                    x.name[0] == "1" ||
                    x.name[0] == "2" ||
                    x.name[0] == "E"
                );
                floorObjects = objectArr.filter(
                    x =>
                    x.name[0] == "c" ||
                    x.name[0] == "s" ||
                    x.name[0] == "f" ||
                    x.name[0] == "g"
                );
                console.log("Loading complete!");
                console.log(objectArr);
            }
            var mtlLoader = new THREE.MTLLoader();
            mtlLoader.setPath("models/");
            var url = "model" + ".mtl"; //modelName
            mtlLoader.load(url, function (materials) {
                materials.preload();

                var objLoader = new THREE.OBJLoader(manager);
                objLoader.setMaterials(materials);
                objLoader.setPath("models/");
                objLoader.load("model" + ".obj", function (object) {
                    scene.add(object);
                    for (var i = 0; i < object.children.length; i++) {
                        objectArr.push(object.children[i]);
                        if (floors.includes(object.children[i].name)) {
                            object.children[i].castShadow = true;
                        } else {
                            object.children[i].receiveShadow = true;
                        }
                    }

                    scene.traverse(function (children) {
                        objects.push(children);
                    });
                });
			});
}