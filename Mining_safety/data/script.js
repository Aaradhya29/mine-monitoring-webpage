// Get current sensor readings when the page loads  
window.addEventListener('load', getReadings);

// Create Temperature Gauge
var gaugeTemp = new LinearGauge({
  renderTo: 'gauge-temperature',
  width: 120,
  height: 400,
  units: "Temperature C",
  minValue: 0,
  startAngle: 90,
  ticksAngle: 180,
  maxValue: 60,
  colorValueBoxRect: "#049faa",
  colorValueBoxRectEnd: "#049faa",
  colorValueBoxBackground: "#f1fbfc",
  valueDec: 2,
  valueInt: 2,
  majorTicks: [
      "0",
      "5",
      "10",
      "15",
      "20",
      "25",
      "30",
      "35",
      "40",
      "45",
      "50",
      "55",
      "60"
  ],
  minorTicks: 4,
  strokeTicks: true,
  highlights: [
      {
          "from": 40,
          "to": 60,
          "color": "#c83232"
      }
  ],
  colorPlate: "#fff",
  colorBarProgress: "#CC2936",
  colorBarProgressEnd: "#049faa",
  borderShadowWidth: 0,
  borders: false,
  needleType: "arrow",
  needleWidth: 5,
  needleCircleSize: 7,
  needleCircleOuter: true,
  needleCircleInner: false,
  animationDuration: 1500,
  animationRule: "linear",
  barWidth: 5,
}).draw();
  
// Create Humidity Gauge
var gaugeHum = new RadialGauge({
  renderTo: 'gauge-humidity',
  width: 250,
  height: 300,
  units: "Humidity (%)",
  minValue: 0,
  maxValue: 100,
  colorValueBoxRect: "#049faa",
  colorValueBoxRectEnd: "#049faa",
  colorValueBoxBackground: "#f1fbfc",
  valueInt: 2,
  majorTicks: [
      "0",
      "20",
      "40",
      "60",
      "80",
      "100"

  ],
  minorTicks: 4,
  strokeTicks: true,
  highlights: [
      {
          "from": 80,
          "to": 100,
          "color": "#C83232"
      }
  ],
  colorPlate: "#fff",
  borderShadowWidth: 0,
  borders: false,
  needleType: "line",
  colorNeedle: "#007F80",
  colorNeedleEnd: "#007F80",
  needleWidth: 2,
  needleCircleSize: 3,
  colorNeedleCircleOuter: "#007F80",
  needleCircleOuter: true,
  needleCircleInner: false,
  animationDuration: 1500,
  animationRule: "linear"
}).draw();

// Create Pressure Gauge
var gaugePre = new RadialGauge({
    renderTo: 'gauge-pressure',
    width: 250,
    height: 300,
    units: "Pressure (hPa)",
    minValue: 0,
    maxValue: 1200,
    colorValueBoxRect: "#049faa",
    colorValueBoxRectEnd: "#049faa",
    colorValueBoxBackground: "#f1fbfc",
    valueInt: 2,
    majorTicks: [
        "0",
        "200",
        "400",
        "600",
        "800",
        "1000",
        "1200"
  
    ],
    minorTicks: 4,
    strokeTicks: true,
    highlights: [
        {
            "from": 800,
            "to": 1200,
            "color": "#C83232"
        }
    ],
    colorPlate: "#fff",
    borderShadowWidth: 0,
    borders: false,
    needleType: "line",
    colorNeedle: "#007F80",
    colorNeedleEnd: "#007F80",
    needleWidth: 2,
    needleCircleSize: 3,
    colorNeedleCircleOuter: "#007F80",
    needleCircleOuter: true,
    needleCircleInner: false,
    animationDuration: 1500,
    animationRule: "linear"
  }).draw();

// Create MQ4 Gas concentration Gauge
var gaugeMQ4 = new RadialGauge({
    renderTo: 'gauge-concentration 1',
    width: 250,
    height: 300,
    units: "Gas Concentration (ppm)",
    minValue: 0,
    maxValue: 10000,
    colorValueBoxRect: "#049faa",
    colorValueBoxRectEnd: "#049faa",
    colorValueBoxBackground: "#f1fbfc",
    valueInt: 2,
    majorTicks: [
        "0",
        "1000",
        "2000",
        "3000",
        "4000",
        "5000",
        "6000",
        "7000",
        "8000",
        "9000",
        "10000"
  
    ],
    minorTicks: 4,
    strokeTicks: true,
    highlights: [
        {
            "from": 8000,
            "to": 10000,
            "color": "#C83232"
        }
    ],
    colorPlate: "#fff",
    borderShadowWidth: 0,
    borders: false,
    needleType: "line",
    colorNeedle: "#007F80",
    colorNeedleEnd: "#007F80",
    needleWidth: 2,
    needleCircleSize: 3,
    colorNeedleCircleOuter: "#007F80",
    needleCircleOuter: true,
    needleCircleInner: false,
    animationDuration: 1500,
    animationRule: "linear"
  }).draw();

// Create MQ9 Gas concentration Gauge
var gaugeMQ9 = new RadialGauge({
    renderTo: 'gauge-concentration 2',
    width: 250,
    height: 300,
    units: "Gas Concentration (ppm)",
    minValue: 0,
    maxValue: 10000,
    colorValueBoxRect: "#049faa",
    colorValueBoxRectEnd: "#049faa",
    colorValueBoxBackground: "#f1fbfc",
    valueInt: 2,
    majorTicks: [
        "0",
        "1000",
        "2000",
        "3000",
        "4000",
        "5000",
        "6000",
        "7000",
        "8000",
        "9000",
        "10000"
  
    ],
    minorTicks: 4,
    strokeTicks: true,
    highlights: [
        {
            "from": 8000,
            "to": 10000,
            "color": "#C83232"
        }
    ],
    colorPlate: "#fff",
    borderShadowWidth: 0,
    borders: false,
    needleType: "line",
    colorNeedle: "#007F80",
    colorNeedleEnd: "#007F80",
    needleWidth: 2,
    needleCircleSize: 3,
    colorNeedleCircleOuter: "#007F80",
    needleCircleOuter: true,
    needleCircleInner: false,
    animationDuration: 1500,
    animationRule: "linear"
  }).draw();
  
// Function to get current readings on the webpage when it loads for the first time
function getReadings(){
  var xhr = new XMLHttpRequest();
  xhr.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      var myObj = JSON.parse(this.responseText);
      console.log(myObj);
      var temp = myObj.temperature;
      var hum = myObj.humidity;
      var pre = myObj.pressure;
      var mq4 = myObj.mq4;
      var mq9 = myObj.mq9;
      var lat = myObj.latitude;
      var long = myObj.longitude;
      
    
      gaugeTemp.value = temp;
      gaugeHum.value = hum;
      gaugePre.value = pre;
      gaugeMQ4.value = mq4;
      gaugeMQ9.value = mq9;
      gaugeLong.value = long;
      gaugeLat.value = lat;
      
    }
  }; 
  xhr.open("GET", "/readings", true);
  xhr.send();
}

if (!!window.EventSource) {
  var source = new EventSource('/events');
  
  source.addEventListener('open', function(e) {
    console.log("Events Connected");
  }, false);

  source.addEventListener('error', function(e) {
    if (e.target.readyState != EventSource.OPEN) {
      console.log("Events Disconnected");
    }
  }, false);
  
  source.addEventListener('message', function(e) {
    console.log("message", e.data);
  }, false);
  
  source.addEventListener('new_readings', function(e) {
    console.log("new_readings", e.data);
    var myObj = JSON.parse(e.data);
    console.log(myObj);
    gaugeTemp.value = myObj.temperature;
    gaugeHum.value = myObj.humidity;
    gaugePre.value = myObj.pressure;
    gaugeMQ4.value = myObj.mq4;
    gaugeMQ9.value = myObj.mq9;
    gaugeLong.value = myObj.long;
    gaugeLat.value = myObj.lat;
    
  }, false);
}