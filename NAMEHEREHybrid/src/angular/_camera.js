/**
* Camera module for Angular
* @module angular/camera
*/
angular.module('namehere.camera', [])

    .factory('$namehereCamera', [function ($q) {

        return {
            /**
            * Capture a picture
            * @instance
            *
            * @arg {Object} options - Options
            */
            getPicture: function (options) {
                return null;
            },

            /**
            * Clean up cached data
            * @instance
            */
            cleanup: function () {
                return null;
            }
        };
    }]);