from flask import Flask, render_template, url_for, request, redirect
from flask_json import FlaskJSON, jsonify
import requests

app = Flask(__name__)
json = FlaskJSON(app)

@app.route("/")
def index():
    return render_template("index.html")

@app.route("/allen-building")
def allen():
    return render_template("allen.html")

@app.route("/full")
def full():
    r=requests.post("http://192.168.4.3/set-position", headers={'Content-Type':'application/json'}, json={'id':'usa','pos':1})
    return render_template("allen.html")

@app.route("/half")
def half():
    r=requests.post("http://192.168.4.3/set-position", headers={'Content-Type':'application/json'}, json={'id':'usa','pos':0})
    return render_template("allen.html")

@app.route("/white-lecture-hall")
def wlh():
    return render_template("wlh.html")

if __name__ == "__main__":
    app.run(debug=True)