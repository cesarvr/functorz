'use strict';

(function() {
    function createFn() {
        return {
            code: $('#code').val(),
            name: $('#fname').val()
        };
    }


    $('#send').click(function(e) {
        e.preventDefault();
        var $console = $('.result');

        var compiling = $.post('/function', createFn());
        
        compiling.done(function(msg){
          $console.removeClass('fail').addClass('sucess').html(msg.result);
        });

        compiling.fail(function(msg){
          var result = JSON.parse(msg.responseText).result; 
          $console.removeClass('sucess').addClass('fail').html(result);
        });


        console.log('boom')
    });
})();
