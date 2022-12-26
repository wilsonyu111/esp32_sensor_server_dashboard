function updateSliderPWM(element) {
  const sliderValue = document.getElementById("pwmSlider").value;
  document.getElementById("textSliderValue").innerHTML = sliderValue;
  const request = new XMLHttpRequest();
  request.open("GET", "/slider?value=" + sliderValue, true);
  request.send();
}

function autoMode(element) {
  const request = new XMLHttpRequest();
  request.open("GET", "/mode?value=auto", true);
  request.send();
  document.getElementById("current_mode").innerHTML = "curtain mode: auto";
}

function manualMode(element) {
  const request = new XMLHttpRequest();
  request.open("GET", "/mode?value=manual", true);
  request.send();
  document.getElementById("current_mode").innerHTML = "curtain mode: manual";
}

function updatePageValue(element) {
  const request = new XMLHttpRequest();
  request.addEventListener("readystatechange", () => {
    // in async request, ready state 4 is when the request is fully done
    // look at xml readystatechange for what each code means
    if (request.readyState === 4) {
      const data = request.responseText;
      const dataMap = new Map(Object.entries(JSON.parse(data)));
      console.log(dataMap);
      document.getElementById("textSliderValue").innerHTML =
        dataMap.get("curtain_position");
      document.getElementById("current_mode").innerHTML =
        "current mode: " + dataMap.get("curtain_mode");
      document.getElementById("pwmSlider").value = parseInt(
        dataMap.get("curtain_position")
      );
    }
  });
  request.open("GET", "/status", true);
  request.send();
}
