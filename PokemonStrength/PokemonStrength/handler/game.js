var StaticServlet = require('../staticHandler'),
    readTemplate = require('../readTemplate').readTemplate;

var GameHandler = new StaticServlet();

GameHandler.handleRequest = function (req, res) {
    res.writeHead(200, {
        'Content-Type': 'text/html'
    });
    this.writeHtmlHead(res);
    res.write(readTemplate("pkmGame", {}));
    this.writeHtmlFooter(res);
}


exports.GameHandler = GameHandler;