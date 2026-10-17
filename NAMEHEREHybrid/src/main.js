//
// NAMEHEREHybrid
//

//---------------------------------------------------
define('NAMEHEREHybrid', {}, function(require, exports, module) {
    exports.Utils = require("Utils");
    exports.Events = require("Events");
    exports.Bridge = require("Bridge");
    exports.Game = require("Game");
    exports.Social = require("Social");
    exports.Chat = require("Chat");
    exports.Input = require("Input");
});

//---------------------------------------------------
// Define a global namehere scope
if(!window.NAMEHERE) {
    window.NAMEHERE = {};
}
window.NAMEHERE.Hybrid = require('NAMEHEREHybrid');
    