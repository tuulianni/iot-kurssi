/*const express = require("express");
const app = express();
const path = require("path");
const IP = require("ip");
 const { json } = require("express");
const fs = require("fs");

var wantsJson = { temperature_target: 25, humidity_target: 50 };
app.use(express.json({ limit: "3mb" }));

app.get("/api/targets", function (req, res) {
  //res.send();
  const ipAddress = IP.address();
  console.log(ipAddress);
  let targets = fs.readFileSync("/targets.txt", "utf8");
  let targets_json = JSON.parse(targets)
  res.json({
    targets_json
    //temperature_target: "23",
    //humidity_target: "50",
  });
  res.end();
});
app.post("/data", function (req, res) {
  let arvot = fs.readFileSync("/dataBase.txt", "utf8");

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
  //res.sendFile(path.join(__dirname, '/index.html'));
  res.sendFile(path.join(__dirname, "/web/index.html"));

  //res.end();
});

app.post("/api/set_targets", function (req, res) {
  let wants_humidity = req.body.wants_humidity;
  let wants_temperature = req.body.wants_temperature;
  console.log(wants_humidity);
  console.log(wants_temperature);
  //console.log(req.body.wants_temperature);
  //console.log(req.body.wants_humidity);
  if (wants_humidity != "-99") {
    wantsJson["humidity_target"] = wants_humidity;
  }
  if (wants_temperature != "-99") {
    wantsJson["temperature_target"] = wants_temperature;
  }

  fs.writeFile("/home/admin/server/targets.txt", JSON.stringify(wantsJson), (err) => {
    if (err) {
      console.log(err);
    }
  });
  console.log("wantsJson.temperature_target = " + wantsJson.temperature_target);
  console.log("wantsJson.humidity_target = " + wantsJson.humidity_target);
});
app.post("/api/upload", function (req, res) {
  let humidity = req.body.humidity;
  let temperature = req.body.temperature;
  let valuesJson = JSON.stringify({
    temperature: temperature,
    humidity: humidity,
  });
  console.log(typeof valuesJson);
  console.log('"Temperature: ' + temperature + '"');
  console.log('"Humidity: ' + humidity + '"');
  fs.writeFile("/dataBase.txt", valuesJson, (err) => {
    if (err) {
      console.log(err);
    }
  });
  //console.log(req.body.name);
  //console.log(typeof req.body)
  //res.json({
  //     Vastaus: 'ON'
  //});
});

const server = app.listen(8080, function () {
  const host = server.address().address;
  const port = server.address().port;

  console.log("http://%s:%s", host, port);
});*/