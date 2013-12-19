// FileSystem
var fs = require("fs");

function reEscape(s) {
    return s.replace(/([.*+?^$|(){}\[\]])/mg, "\\$1");
}

// Template name is the filename of the html file
// objvar 
function readTemplate(templateName, objVar, allowMissing)
{
    var result;
    var filePath = "template/" + templateName + ".html";
    try
    {
        var data = fs.readFileSync(filePath, 'utf-8');

        // Match words
        {
            var pattern = /##[a-z0-9]##/i;
            var exp = pattern.exec(data);
            while (exp) {
                var patt = new RegExp(exp[0], 'g');
                var newContent = objVar[exp[1]];

                if (newContent === undefined) {
                    throw "Missing variable " + exp[1] + " in template variables";
                }
                data = data.replace(patt, newContent);

                exp = pattern.exec(data);
            }
        }

        {
            var pattern = /##(.*)##/;
            var exp = pattern.exec(data);
            while (exp) {
                var patt = new RegExp(reEscape(exp[0]), 'g');
                var newContent = objVar[exp[1]];

                if (newContent === undefined) {
                    newContent = eval(exp[1]);
                }
                data = data.replace(patt, newContent);

                exp = pattern.exec(data);
            }
        }

        result = data;
    }
    catch(err)
    {
        //error
        result = "<div>Error reading template " + filePath + "<br/>" + err + "</div>";
    }
    return result;
}


console.log(readTemplate("pkmGame", {}, true));
