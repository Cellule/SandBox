
/*
 * GET type game page.
 */
global.typeGameImg = global.imgpath+"typegame/";

exports.typeGame = function(req, res){
    res.render('typeGame', { title: 'Pokemon Type Game' });
};