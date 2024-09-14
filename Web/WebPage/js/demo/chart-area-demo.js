// Set new default font family and font color to mimic Bootstrap's default styling
Chart.defaults.global.defaultFontFamily = 'Nunito', '-apple-system,system-ui,BlinkMacSystemFont,"Segoe UI",Roboto,"Helvetica Neue",Arial,sans-serif';
Chart.defaults.global.defaultFontColor = '#858796';
Chart.defaults.global.tooltips.enabled = false;


var xValues = [];
var maxElements = 3400
var options = {
  animation: {
    duration: 0
  },
  maintainAspectRatio: false,
  layout: {
    padding: {
      left: 10,
      right: 25,
      top: 25,
      bottom: 0
    }
  },
  scales: {
    xAxes: [{
      min: 0, max:3600,
      gridLines: {
        display: false,
        drawBorder: false
      },
      display: false
    }],
    yAxes: [{
      ticks: {
        max: 16,
        maxTicksLimit: 10,
        padding: 10,
        // Include a dollar sign in the ticks
      },
      gridLines: {
        color: "rgb(234, 236, 244)",
        zeroLineColor: "rgb(234, 236, 244)",
        drawBorder: false,
        borderDash: [2],
        zeroLineBorderDash: [2]
      }
    }],
  },
  legend: {
    display: false
  },
  plugins: {
    tooltip: {
      enabled: false,
      callbacks: {
          title : () => null // or function () { return null; }
      }
    },
  },
  hover: {mode: null},
  events: [],
  showTooltips: false
  
}
setInterval(getLastData, 1000);

// Area Chart Example
var ctx = document.getElementById("myAreaChart");
var myLineChart = new Chart(ctx, {
  type: 'line',
  data: {
    labels: xValues,
    datasets: [{
      lineTension: 0.3,
      backgroundColor: "rgba(78, 115, 223, 0.05)",
      borderColor: "rgba(78, 115, 223, 1)",
      pointRadius: 0,
      pointBackgroundColor: "rgba(78, 115, 223, 1)",
      pointBorderColor: "rgba(78, 115, 223, 1)",
      pointHoverRadius: 3,
      pointHoverBackgroundColor: "rgba(78, 115, 223, 1)",
      pointHoverBorderColor: "rgba(78, 115, 223, 1)",
      pointBorderWidth: 2,
      data: xValues,
    }],
  },
  options: options
});
i = 0
function getLastData() {
  fetch("http://127.0.0.1:5000/update")
  .then(response => response.json())
  .then(json => useData(json))

}

function useData(json) {
  state = json['state'];
  water = parseFloat(json['water']).toFixed(1);
  timer = json['timer'];
  errorCode = json['error'];
  voltage = json['voltage'].toFixed(1);

  // Voltage
  if(xValues.length > maxElements) {
    xValues = xValues.slice(1);
  }
  xValues.push(voltage);
  myLineChart = new Chart(ctx, {
    type: 'line',
    data: {
      labels: xValues,
      datasets: [{
        borderWidth: 1,
        lineTension: 0.3,
        backgroundColor: "rgba(78, 115, 223, 0.05)",
        borderColor: "rgba(78, 115, 223, 1)",
        pointRadius: 0,
        pointBackgroundColor: "rgba(78, 115, 223, 1)",
        pointBorderColor: "rgba(78, 115, 223, 1)",
        pointHoverRadius: 3,
        pointHoverBackgroundColor: "rgba(78, 115, 223, 1)",
        pointHoverBorderColor: "rgba(78, 115, 223, 1)",
        pointBorderWidth: 2,
        data: xValues,
      }],
    },
    options: options
  });

  document.getElementById('voltageCard').innerText = voltage + 'v';
  document.getElementById('waterCard').innerText = water + '%';
  document.getElementById('timerCard').innerText = timer;
  document.getElementById('stateCard').innerText = state;
  document.getElementById('errorText').innerText = "Error " + errorCode;
  errorCard = document.getElementById('errorCard');

  switch(errorCode) {
    case "1": 
      errorCard.style.display = "";
      errorText = "La génératrice n'as pas démarré !";
      document.getElementById('errorText').innerText = "\t Error " + errorCode + ": " + errorText;
      break;
    case "2": 
      errorCard.style.display = "";
      errorText = "La génératrice s'est éteinte en cours de marche !";
      document.getElementById('errorText').innerText = "\t Error " + errorCode + ": " + errorText;
      break;
    case "3": 
      errorCard.style.display = "";
      errorText = "La clef n'a pas pu être mise à OFF !";
      document.getElementById('errorText').innerText = "\t Error " + errorCode + ": " + errorText;
      break;
      default:
        errorCard.style.display = "none";
  }
}