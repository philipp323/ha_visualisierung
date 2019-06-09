		//move floors and enable/disable rooms on floors
		async function floorSelect(name) {
      if (movingIndex == floors.indexOf(name) || currentlyMoving) {
          return;
        }
        currentlyMoving = true;
        movingIndex = floors.indexOf(name);
        if(floorButton != null){
          floorButton.style.backgroundColor = "";
          floorButton.style.color = "white";
        }
        floorButton = document.getElementById(name);
        floorButton.style.backgroundColor = "DodgerBlue";
        floorButton.style.color = "white";
  
        // move down
        for (var i = objectsUp[0]; i <= movingIndex; i++) {
            objectArr.find(x => x.name == floors[i]).visible = true;
          }
  
        if (objectsUp.includes(movingIndex) && floors.includes(name)) {
          for (var j = 0; j <= 50; j++) {
            for (var i = objectsUp[0]; i <= movingIndex; i++) {
              if (objectsUp.includes(i)) {
                await move("down", floors[i]);
              }
            }
  
          }
  
  
  
          //enable rooms
          var enabledRooms = [];
          for (var i = objectsUp[0]; i <= movingIndex; i++) {
            var helpChar;
            if(floors[i].toString() == floors[0]){
              helpChar = "U";
            }
            if(floors[i].toString() == floors[1]){
              helpChar = "E";
            }
            if(floors[i].toString() == floors[2]){
              helpChar = "1";
            }
            if(floors[i].toString() == floors[3]){
              helpChar = "2";
            }
            enabledRooms = objectArr.filter(x => x.name[0] == helpChar && x.name != floors[0] && x.name != floors[1] && x.name != floors[2] && x.name != floors[3]);
  
            for (let j = 0; j < enabledRooms.length; j++) {
              var objectEnable = objectArr.find(x => x.name == enabledRooms[j].name);
              objectEnable.visible = true;
            }
          }
  
          for (var i = 0; i <= movingIndex; i++) {
            objectsUp.splice(objectsUp.indexOf(movingIndex), 1);
          }

          if(objectSelected != null){
              objectSelected.material.color.setHex(0xffffff);
              objectSelected = null;
          }

        }
        // move up
        else if (floors.includes(name)) {
          //disable rooms
          var disabledRooms = [];
          for (let i = floors.length - 1; i > movingIndex; i--) {
            var helpChar;
            if(floors[i].toString() == floors[0]){
              helpChar = "U";
            }
            if(floors[i].toString() == floors[1]){
              helpChar = "E";
            }
            if(floors[i].toString() == floors[2]){
              helpChar = "1";
            }
            if(floors[i].toString() == floors[3]){
              helpChar = "2";
            }
            disabledRooms = objectArr.filter(x => x.name[0] == helpChar && x.name != floors[0] && x.name != floors[1] && x.name != floors[2] && x.name != floors[3]);
            for (let j = 0; j < disabledRooms.length; j++) {
              var objectDisable = objectArr.find(x => x.name == disabledRooms[j].name);
              objectDisable.visible = false;
            }
          }
  
  
          for (let j = 0; j <= 50; j++) {
            for (let i = floors.length - 1; i > movingIndex; i--) {
              if (!objectsUp.includes(i)) {
                await move("up", floors[i]);
              }
            }
          }
  
          for (let i = floors.length - 1; i > movingIndex; i--) {
                      objectArr.find(x => x.name == floors[i]).visible = false;
          }
        }
  
  
        for (let i = floors.length - 1; i > movingIndex; i--) {
          if (!objectsUp.includes(i)) {
            objectsUp.push(i);
          }
  
        }
        objectsUp.sort();
  
        currentlyMoving = false;
  
      }