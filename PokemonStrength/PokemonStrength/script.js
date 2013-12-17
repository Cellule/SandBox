if (!String.prototype.format) {
    String.prototype.format = function () {
        var args = arguments;
        return this.replace(/{(\d+)}/g, function (match, number) {
            return typeof args[number] != 'undefined'
              ? args[number]
              : match
            ;
        });
    };
}
var debug = true;

var imgPath = "res/img/";

var pkmTypesEnum = {
    TypeNormal  : 0 ,
    TypeFire    : 1 ,
    TypeWater   : 2 ,
    TypeElectric: 3 ,
    TypeGrass   : 4 ,
    TypeIce     : 5 ,
    TypeFighting: 6 ,
    TypePoison  : 7 ,
    TypeGround  : 8 ,
    TypeFlying  : 9 ,
    TypePsychic : 10,
    TypeBug     : 11,
    TypeRock    : 12,
    TypeGhost   : 13,
    TypeDragon  : 14,
    TypeDark    : 15,
    TypeSteel   : 16
};

var Normal = {
    name: "Normal",
    type: pkmTypesEnum.TypeNormal,
    imgsrc: imgPath+"normal.png",
    weakness: [1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1]
};
var Fire = {
    name: "Fire",
    type: pkmTypesEnum.TypeFire,
    imgsrc: imgPath+"fire.png",
    weakness: [1, 0.5, 2, 1, 0.5, 0.5, 1, 1, 2, 1, 1, 0.5, 2, 1, 1, 1, 0.5]
};
var Water = {
    name: "Water",
    type: pkmTypesEnum.TypeWater,
    imgsrc: imgPath+"water.png",
    weakness: [1, 0.5, 0.5, 2, 2, 0.5, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0.5]
};
var Electric = {
    name: "Electric",
    type: pkmTypesEnum.TypeElectric,
    imgsrc: imgPath+"electric.png",
    weakness: [1, 1, 1, 0.5, 1, 1, 1, 1, 2, 0.5, 1, 1, 1, 1, 1, 1, 0.5]
};
var Grass = {
    name: "Grass",
    type: pkmTypesEnum.TypeGrass,
    imgsrc: imgPath+"grass.png",
    weakness: [1, 2, 0.5, 0.5, 0.5, 2, 1, 2, 0.5, 2, 1, 2, 1, 1, 1, 1, 1]
};
var Ice = {
    name: "Ice",
    type: pkmTypesEnum.TypeIce,
    imgsrc: imgPath+"ice.png",
    weakness: [1, 2, 1, 1, 1, 0.5, 2, 1, 1, 1, 1, 1, 2, 1, 1, 1, 2]
};
var Fighting = {
    name: "Fighting",
    type: pkmTypesEnum.TypeFighting,
    imgsrc: imgPath+"fighting.png",
    weakness: [1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 0.5, 0.5, 1, 1, 0.5, 1]
};
var Poison = {
    name: "Poison",
    type: pkmTypesEnum.TypePoison,
    imgsrc: imgPath+"poison.png",
    weakness: [1, 1, 1, 1, 0.5, 1, 0.5, 0.5, 2, 1, 2, 0.5, 1, 1, 1, 1, 1]
};

var Ground = {
    name: "Ground",
    type: pkmTypesEnum.TypeGround,
    imgsrc: imgPath+"ground.png",
    weakness: [1, 1, 2, 0, 2, 2, 1, 0.5, 1, 1, 1, 1, 0.5, 1, 1, 1, 1]
};
var Flying = {
    name: "Flying",
    type: pkmTypesEnum.TypeFlying,
    imgsrc: imgPath+"flying.png",
    weakness: [1, 1, 1, 2, 0.5, 2, 0.5, 1, 0, 1, 1, 0.5, 2, 1, 1, 1, 1]
};
var Psychic = {
    name: "Psychic",
    type: pkmTypesEnum.TypePsychic,
    imgsrc: imgPath+"psychic.png",
    weakness: [1, 1, 1, 1, 1, 1, 0.5, 1, 1, 1, 0.5, 2, 1, 2, 1, 2, 1]
};
var Bug = {
    name: "Bug",
    type: pkmTypesEnum.TypeBug,
    imgsrc: imgPath+"bug.png",
    weakness: [1, 2, 1, 1, 0.5, 1, 0.5, 1, 0.5, 2, 1, 1, 2, 1, 1, 1, 1]
};
var Rock = {
    name: "Rock",
    type: pkmTypesEnum.TypeRock,
    imgsrc: imgPath+"rock.png",
    weakness: [0.5, 0.5, 2, 1, 2, 1, 2, 0.5, 2, 0.5, 1, 1, 1, 1, 1, 1, 2]
};
var Ghost = {
    name: "Ghost",
    type: pkmTypesEnum.TypeGhost,
    imgsrc: imgPath+"ghost.png",
    weakness: [0, 1, 1, 1, 1, 1, 0, 0.5, 1, 1, 1, 0.5, 1, 2, 1, 2, 1]
};
var Dragon = {
    name: "Dragon",
    type: pkmTypesEnum.TypeDragon,
    imgsrc: imgPath+"dragon.png",
    weakness: [1, 0.5, 0.5, 0.5, 0.5, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1]
};
var Dark = {
    name: "Dark",
    type: pkmTypesEnum.TypeDark,
    imgsrc: imgPath+"dark.png",
    weakness: [1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 0, 2, 1, 0.5, 1, 0.5, 1]
};
var Steel = {
    name: "Steel",
    type: pkmTypesEnum.TypeSteel,
    imgsrc: imgPath+"steel.png",
    weakness: [0.5, 2, 1, 1, 0.5, 0.5, 2, 0, 2, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5]
};


var pkmTypes = [
Normal,
Fire,
Water,
Electric,
Grass,
Ice,
Fighting,
Poison,
Ground,
Flying,
Psychic,
Bug,
Rock,
Ghost,
Dragon,
Dark,
Steel
];




var nbType = pkmTypes.length;

if (debug) {
    for (i in pkmTypes) {
        var p = pkmTypes[i];
        if (p.weakness.length != nbType) {
            alert("Error: {0} weakness list ({1}) doesn't match expected size ({2})".format(p.name, p.weakness.length, nbType));
        }
    }

    for (i in pkmTypes) {
        var p = pkmTypes[i];
        for (j in pkmTypes) {
            var p2 = pkmTypes[i];

            for (k in pkmTypes) {
                var v = p.weakness[k] * p2.weakness[k];
                if ( v < 0 || v > 4 )
                {
                    alert("Error: combo {0}&{1} weakness {2} equals {3}".format(p.name, p2.name, pkmTypes[k].name, v));
                }
            }
        }
    }
}

function chooseRandPkmType()
{
    filter = null;
    if (arguments.length > 0)
    {
        filter = arguments[0];
    }

    type = null;
    while(!type || filter == type)
    {
        var a = Math.round(Math.random() * nbType);
        type = pkmTypes[a];
    }
    return type;
}