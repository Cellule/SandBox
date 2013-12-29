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

function chooseRandPkmType(pkmTypes)
{
    var nbType = pkmTypes.length;
    var filter = null;
    if (arguments.length > 1)
    {
        filter = arguments[1];
    }

    var type = null;
    while(!type || filter == type)
    {
        var a = Math.round(Math.random() * nbType);
        type = pkmTypes[a];
    }
    return type;
}