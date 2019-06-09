		//translate function for floors
		async function move(direction, object) {
			mov = objectArr.find(x => x.name === object);
			//console.log("Currently moving: " + mov.name);
			// console.log(mov);
			//await sleep(0.1);
			if (direction == "up") {
				mov.translateY(50);
			}
			else if (direction == "down") {
				mov.translateY(-50);
			}

		}