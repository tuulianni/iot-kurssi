const express = require("express");
const app = express();
const path = require("path");
const IP = require("ip");
const { json } = require("express");
const fs = require("fs");

var servo = { tell_servo: 1 };
app.use(express.json({ limit: "3mb" }));

app.get("/api/orders", function (req, res) {
  //res.send();
  const ipAddress = IP.address();
  console.log(ipAddress);
  let orders = fs.readFileSync(path.join(__dirname, "servocontrol.txt"), "utf8");
  let orders_json = JSON.parse(orders)
  res.json({
    orders_json
  });
  res.end();
});
app.post("/data", function (req, res) {
  let arvot = fs.readFileSync(path.join(__dirname, "temphum.txt"), "utf8");

  console.log("arvot: ", arvot);
  res.json({
    arvot,
  });
  res.end();
});
app.get("/", function (req, res) {
  const ipAddress = IP.address();
  console.log(ipAddress);
  console.log(__dirname);
  res.sendFile(path.join(__dirname, "/web/index.html"));

  //res.end();
});

app.post("/api/give_orders", function (req, res) {
  let servo_order = req.body.servo_order;
  console.log(servo_order);

  fs.writeFile(path.join(__dirname, "servocontrol.txt"), JSON.stringify(servo), (err) => {
    if (err) {
      console.log(err);
    }
  });
  console.log("servo.tell_servo = " + servo.tell_servo);
});
app.post("/api/upload", function (req, res) {
	console.log("vittu tää toimii");
  let humidity = req.body.humidity;
  let temperature = req.body.temperature;
  let valuesJson = JSON.stringify({
    temperature: temperature,
    humidity: humidity,
  });
  console.log(typeof valuesJson);
  console.log('"Temperature: ' + temperature + '"');
  console.log('"Humidity: ' + humidity + '"');
  fs.writeFile(path.join(__dirname, "temphum.txt"), valuesJson, (err) => {
    if (err) {
      console.log(err);
    }
  });

});

const server = app.listen(8080, function () {
  const host = server.address().address;
  const port = server.address().port;

  console.log("http://%s:%s", host, port);
});