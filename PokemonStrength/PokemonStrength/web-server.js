#!/usr/bin/env node

var util = require('util'),
    http = require('http'),
    url = require('url'),
    routes = require('./routes'),
    StaticServlet = require('./staticHandler');

var DEFAULT_PORT = 8000;
var staticHandler = {
    'GET': createServlet(StaticServlet),
    'HEAD': createServlet(StaticServlet)
}

function main(argv) {
    new HttpServer(routes.getHandlers()).start(Number(argv[2]) || DEFAULT_PORT);
}


function createServlet(Class) {
    var servlet = new Class();
    return servlet.handleRequest.bind(servlet);
}

/**
 * An Http server implementation that uses a map of methods to decide
 * action routing.
 *
 * @param {Object} Map of method => Handler function
 */
function HttpServer(handlers) {
    this.handlers = handlers;
    this.server = http.createServer(this.handleRequest_.bind(this));
}

HttpServer.prototype.start = function(port) {
    this.port = port;
    this.server.listen(port);
    util.puts('Http Server running at http://localhost:' + port + '/');
};

HttpServer.prototype.parseUrl_ = function(urlString) {
    var parsed = url.parse(urlString);
    parsed.pathname = url.resolve('/', parsed.pathname);
    return url.parse(url.format(parsed), true);
};

HttpServer.prototype.handleRequest_ = function(req, res) {
    var logEntry = req.method + ' ' + req.url;
    if (req.headers['user-agent']) {
        logEntry += ' ' + req.headers['user-agent'];
    }
    util.puts(logEntry);
    req.url = this.parseUrl_(req.url);
    var handler = null;
    var methodHandlers = this.handlers[req.method];
    if(methodHandlers)
    {

        for(i in methodHandlers)
        {
            h = methodHandlers[i];
            if(typeof(h.route)==='string')
            {
                if(h.route == req.url.path)
                    handler = h.handler;
            }
            else
            {
                var check = h.route.exec(req.url.path);
                // check if regex of route is valid
                if(check)
                    // if regex is valid, make sure the whole path is used
                    if(check[0] == req.url.path)
                        handler = h.handler;
            }
            if(handler)
                break;
        }
    }
    
    if (!handler) {
        // if handler not found, check for static handlers
        handler = staticHandler[req.method];
    }

    if(!handler) {
        res.writeHead(501);
        res.end();
    } else {
        handler.call(this, req, res);
    }
};


// Must be last,
main(process.argv);
