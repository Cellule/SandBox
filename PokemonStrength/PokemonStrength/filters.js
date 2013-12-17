'use strict';

/* Filters */

angular.module('gameFilters', []).filter('succesFilter', function () {
  return function(input) {
      return input ? "Success" : "Fail";
  };
}).filter('reverse', function () {
    return function (items) {
        return items.slice().reverse();
    };
});

;
