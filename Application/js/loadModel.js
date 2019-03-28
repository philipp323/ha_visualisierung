
    //load model with materials
		function loadModel(modelName) {
			$("#loadingScreen").fadeOut(1000);
      var manager = new THREE.LoadingManager();
      manager.onStart = function ( url, itemsLoaded, itemsTotal ) {
				console.log( 'Started loading file: ' + url + '.\nLoaded ' + itemsLoaded + ' of ' + itemsTotal + ' files.' );
      };
      
      manager.onLoad = function ( ) {
				$("#loadingScreen").fadeOut(1000);
				document.getElementById("open").style.visibility = "visible";

				roomObjects = objectArr.filter(x => x.name[0] == "U" || x.name[0] == "1"|| x.name[0] == "2" || x.name[0] == "E");
				floorObjects = objectArr.filter(x => x.name[0] == "c" || x.name[0] == "s"|| x.name[0] == "f" || x.name[0] == "g");
				applyFilter('basic');
				console.log( 'Loading complete!');
      };
      
      
      manager.onProgress = function ( url, itemsLoaded, itemsTotal ) {
      
        console.log( 'Loading file: ' + url + '.\nLoaded ' + itemsLoaded + ' of ' + itemsTotal + ' files.' );
				document.getElementById("open").style.visibility = "hidden";
      };
      
      manager.onError = function ( url ) {
      
        console.log( 'There was an error loading ' + url );
      
      };

			var mtlLoader = new THREE.MTLLoader();
			mtlLoader.setPath('models/');
			var url = modelName + ".mtl";
			mtlLoader.load(url, function (materials) {

				materials.preload();

				var objLoader = new THREE.OBJLoader(manager);
				objLoader.setMaterials(materials);
				objLoader.setPath('models/');
				objLoader.load(modelName + ".obj", function (object) {	
					scene.add(object);
					for (var i = 0; i < object.children.length; i++) {
						objectArr.push(object.children[i]);
						if(floors.includes(object.children[i].name)){
							object.children[i].castShadow = true;
						}
						else{
							object.children[i].receiveShadow = true;
						}
					}

					scene.traverse(function (children) {
						objects.push(children);
					});

				});

			});

			
    }
    
    

    