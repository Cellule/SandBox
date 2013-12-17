var myApp = angular.module('myApp', ['gameFilters']);



myApp.controller('typeGameController', function ($scope, $http) {
    $scope.leftType = null;
    $scope.rightType = null;
    $scope.dualType = null;
    $scope.answer = 0;
    $scope.success = null;
    $scope.history = [];
    $scope.historyLimit = 10;
    $scope.imgPath = imgPath;

    $scope.$on('dataloaded', function () {
        setTimeout(function () {
            
        }, 0, false);
    });

    $scope.generateType = function () {
        
        $scope.leftType = chooseRandPkmType();
        $scope.rightType = chooseRandPkmType();
        var r = Math.random();
        var a = $scope.rightType.weakness[$scope.leftType.type];
        if (r >= 0.8)
        {
            $scope.dualType = chooseRandPkmType($scope.rightType);
            a *= $scope.dualType.weakness[$scope.leftType.type];
            if (a < 0 || a> 4) {
                alert("Error: combo {0}&{1} weakness {2} equals {3}".format($scope.rightType.name, $scope.dualType.name, $scope.leftType.name, a));
            }
        }
        else
        {
            $scope.dualType = null;
        }
        
        $scope.solution = a;
    };

    $scope.choose = function(val)
    {
        $scope.answer = val;
        $scope.success = $scope.solution == val;
        $scope.pushHistory();
        $scope.generateType();
    }

    $scope.pushHistory = function()
    {
        var h = {
            leftType : $scope.leftType ,
            rightType: $scope.rightType,
            dualType : $scope.dualType ,
            answer   : $scope.answer   ,
            solution : $scope.solution
        };
        $scope.history.push(h)

    }

    $scope.generateType();

});