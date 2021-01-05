document.getElementById("brightnessLabel").innerText = `Brightness: ${document.getElementById("brightness").value}`

var brightness = document.getElementById("brightness")
brightness.onchange = function(){
    // console.log(document.getElementById("brightness").value)
    document.getElementById("brightnessLabel").innerText = `Brightness: ${document.getElementById("brightness").value}`
    http.open("GET", `/update?brightness=${document.getElementById("brightness").value}`, true)
    http.send()
}

var http = new XMLHttpRequest()

var power = document.getElementById("powerButton")
power.onclick = function(){
    http.open("GET", "/update?mode=0")
    http.send()
}

var submit = document.getElementById("submitButton")
submit.onclick = function(){
    hex = document.getElementById("color").value
    r = parseInt(hex.substring(1, 3), 16)
    g = parseInt(hex.substring(3, 5), 16)
    b = parseInt(hex.substring(5, 7), 16)
    brightness = document.getElementById("brightness").value
    http.open("GET", `/update?mode=1&r=${r}&g=${g}&b=${b}&brightness=${brightness}`, true)
    http.send()
}

var rainbow = document.getElementById("rainbow")
rainbow.onclick = function(){
    brightness = document.getElementById("brightness").value
    http.open("GET", `/update?mode=2&brightness=${brightness}`, true)
    http.send()
}