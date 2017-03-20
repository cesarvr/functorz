(function(n, service, os) {
/*    
		var os = service.os();
    var http = service.http();
    console.log('try to get stuff->', os);

    console.log('cpus ->', os.cpus())

    function internet() {

        var options = {
            host: 'www.google.com',
            port: 80,
            path: '/index.html'
        };

        http.get(options, function(res) {
            console.log("Got response: " + res.statusCode);
        }).on('error', function(e) {
            console.log("Got error: " + e.message);
        });
    };

   */
		console.log('os', os);
		console.log('os.cpus', os.cpus);
		console.log('os.cpus()', os.cpus());

    console.log('this->', this);
  	console.log('os', this.os);
		console.log('os.cpus', this.os.cpus);
		console.log('os.cpus()', this.os.cpus());


	//	internet();

    return n * n;
});
