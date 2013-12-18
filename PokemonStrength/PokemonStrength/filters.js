'use strict';

/* Filters */

angular.module('gameFilters', []).filter('succesFilter', function () {
  return function(input) {
      return input ? '\u2713' : '\u2718';
  };
}).filter('reverse', function () {
    return function (items) {
        return items.slice().reverse();
    };
});

;
