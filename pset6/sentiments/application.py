from flask import Flask, redirect, render_template, request, url_for

import helpers
from analyzer import Analyzer

app = Flask(__name__)

@app.route("/")
def index():
    return render_template("index.html")

@app.route("/search")
def search():

    # validate screen_name
    screen_name = request.args.get("screen_name")
    if not screen_name:
        return redirect(url_for("index"))

    # get screen_name's tweets
    # clean user nickname
    if screen_name.startswith('@'):
        screen_name = screen_name[1:]
    tweets = helpers.get_user_timeline(screen_name, count=100)

    # TODO
    # analyze tweets for emotions
    analizator = Analyzer()
    positive, negative, neutral = analizator.analyze(tweets)

    # generate chart
    chart = helpers.chart(positive, negative, neutral)

    # render results
    return render_template("search.html", chart=chart, screen_name=screen_name)

if __name__ == '__main__':
    app.run(debug = True)