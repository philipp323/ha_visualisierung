function getData() {
  var firstfloor_mqtt = ["e582", "e581", "e59"];
  var secondfloor_mqtt = ["123", "aula"];
  var thirdfloor_mqtt = [];
  var basement_mqtt = [];
  var floors_mqtt = ["basement", "firstfloor", "secondfloor", "thirdfloor"];
  var floorArray = [
    basement_mqtt,
    firstfloor_mqtt,
    secondfloor_mqtt,
    thirdfloor_mqtt
  ];

  var valueArr = [];

  var clientId = "" + Math.floor(Math.random() * 10000000000000001);
  var client = new Paho.MQTT.Client(
    "vm61.htl-leonding.ac.at",
    Number(4200),
    "" + clientId
  );
  client.startTrace();
  client.onConnectionLost = onConnectionLost;
  client.onMessageArrived = onMessageArrived;

  client.connect({
    timeout: 1000,
    userName: "4bhif",
    password: "",
    onSuccess: onConnect,
    useSSL: false
  });
  console.log("attempting to connect...");

  function sleep(milliseconds) {
    return new Promise(resolve => setTimeout(resolve, milliseconds));
  }

  function onConnect() {
    console.log("connection successful");

    for (var i = 0; i < floorArray.length; i++) {
      //console.log(floors_mqtt[i]);
      for (var j = 0; j < floorArray[i].length; j++) {
        //console.log(floorArray[i][j]);
        if (floorArray[i][j] == "aula" || floorArray[i][j] == "123") {
          //console.log(floorArray[i][j]);
          client.subscribe(
            "htlleonding/" +
              floors_mqtt[i] +
              "/" +
              floorArray[i][j] +
              "/beamer/+"
          );
          /*
                if(floorArray[i][j] == "aula"){

                    console.log("htlleonding/"+floors_mqtt[i]+"/"+floorArray[i][j]+"/kaffee/power");
                    client.subscribe("htlleonding/"+floors_mqtt[i]+"/"+floorArray[i][j]+"/kaffee/Power");
                }
                */
        } else {
          client.subscribe(
            "htlleonding/" + floors_mqtt[i] + "/" + floorArray[i][j] + "/pc/+"
          );
        }
      }
    }
    /* Fill FirstFloor with rooms
    for(var i = 0; i < 75;i++){
      if(i<10){
        firstfloor.push("E0"+i);
        console.log("E0"+i);
      }
      else{
        firstfloor.push("E"+i);
        console.log("E"+i);
      }
    }
    firstfloor.push("E581","E582");
    console.log(firstfloor);
      
    */
  }

  function onConnectionLost(responseObject) {
    if (responseObject.errorCode !== 0) {
      console.log("Connection lost:" + responseObject.errorMessage);
      console.log(responseObject);
    }
  }

  function onMessageArrived(message) {
    console.log(
      "Message Arrived:" + message.payloadString + " " + message.destinationName
    );

    var decoded = JSON.parse(message.payloadString.toString());
    var splittedString = message.destinationName.split("/");
    var room = splittedString[splittedString.length - 3];
    var floorName = splittedString[splittedString.length - 4];
    var datatype = splittedString[splittedString.length - 1];
    if (valueArr.find(x => x.topic == message.destinationName)) {
      var index = valueArr.indexOf(
        valueArr.find(x => x.topic == message.destinationName)
      );
      valueArr[index] = {
        value: decoded,
        topic: message.destinationName,
        room: room,
        floor: floorName,
        datatype: datatype
      };
    } else {
      valueArr.push({
        value: decoded,
        topic: message.destinationName,
        room: room,
        floor: floorName,
        datatype: datatype
      });
    }

    if (objectSelected != null) {
      if (filter == "basic") {
        sendName(objectSelected.name);
      }
    }

    //console.log(valueArr);
  }
}
