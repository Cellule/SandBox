var GameHandler = require('./handler/game').GameHandler;




exports.getHandlers = function ()
{
    return {
        'GET': [
            {
                route: "/",
                handler: GameHandler.handleRequest.bind(GameHandler)
            }
        ]
    };
}