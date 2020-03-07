var express = require("express");
var app = express();
var server = require("http").Server(app);
var io = require("socket.io")(server);
var SerialPort = require("serialport");
const Readline = SerialPort.parsers.Readline;
const redis = require("redis");
const client = redis.createClient();
var http = require("http");

// var port = new SerialPort('/dev/ttyS0', {
//   baudRate: 9600
// });
const parser = new Readline();
// port.pipe(parser);
server.listen(8000);

client.on("connect", function () {
    console.log("Redis client connected");
});

client.on("error", function (err) {
    console.log("Something went wrong " + err);
});

app.use(express.static(__dirname + "/"));

app.get("/", function (req, res) {
    res.sendFile(__dirname + "/index.html");
});

io.on("connection", function (socket) {
    app.get("/data/", function (req, res) {
        // res.sendFile(__dirname + "/index.html");
        //sanity check for data
        // console.log(req.query);
        // console.log(req.query.name);
        res.setHeader("Content-Type", "text/plain; charset=utf-8");

        client.hmset(
            "values",
            "position",
            // parseInt(req.query.name)
            req.query.position
        );
        client.hgetall("values", function (err, object) {

            console.log("position " + parseInt(object.position));
            if (parseInt(object.position) >= 50) {
                res.setHeader("Content-Type", "text/plain; charset=utf-8");
                res.end("1");
            } else {
                res.setHeader("Content-Type", "text/plain; charset=utf-8");
                res.end("0");
            }

        });
    });

    //color values from the index page
    parser.on("data", function (data) {});
    socket.on("fromwebclient", function (data) {
        // client.hmset('frameworks', 'javascript', Base64.encode(data.r), 'css', 'Bootstrap', 'node', 'Express');
        client.hmset(
            "colorValues",
            "red",
            parseInt(data.r),
            "green",
            parseInt(data.g),
            "blue",
            parseInt(data.b)
        );
        client.hgetall("colorValues", function (err, object) {
            console.log("red value " + object.red);
            console.log("green value " + object.green);
            console.log("blue value " + object.blue);
            // console.log(data.helloworld);
            // console.log(data.helloworld);
        });

        console.log(data.r + "," + data.g + "," + data.b);
        socket.emit("toscreen", {
            r: data.r,
            g: data.g,
            b: data.b
        });
    });
});

// app.get("/", function(req, res) {
//   res.setHeader("Content-Type", "text/plain; charset=utf-8");
//   console.log("Node Router! --- NOTHING TO SEE HERE");
//   res.end("Node Router! --- NOTHING TO SEE HERE");
// });
// app.get("/value/:value", function(req, res) {
//   console.log(req.params.value);
//   res.end("the value entered is " + req.params.value);
// });