# from cs50 import SQL
import sqlite3
from flask import Flask, flash, redirect, render_template, request, session, url_for, g
from flask_session import Session
from passlib.apps import custom_app_context as pwd_context
from tempfile import gettempdir
from datetime import datetime

from helpers import *

# configure application
app = Flask(__name__)

# ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# custom filter
app.jinja_env.filters["usd"] = usd

# configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = 'session' # use gettempdir() if start server with admin rights
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# configure CS50 Library to use SQLite database
# db = SQL("sqlite:///finance.db")

DATABASE = 'finance.db'

def get_db():
    db = getattr(g, '_database', None)
    if db is None:
        db = g._database = sqlite3.connect(DATABASE)
    return db

@app.teardown_appcontext
def close_connection(exception):
    db = getattr(g, '_database', None)
    if db is not None:
        db.close()

def query_db(query, *args, one=False):
    print(query, args)
    conn = get_db()
    cur = conn.execute(query, (args))
    conn.commit()

    rv = cur.fetchall()
    cur.close()
    # conn.close()
    return (rv[0] if rv else None) if one else rv

@app.route("/", methods=["GET"])
@login_required
def index():
    user_id = session["user_id"]

    response = query_db("SELECT symbol, SUM(amount), SUM(total) from transactions GROUP BY symbol")
    print(response)
    if response:
        symb_dict = dict()
        for symb in response:
            symb_dict[symb[0]] = {'amount': int(symb[1]), 'buy_cost': float(symb[2])/int(symb[1]), 'paid_value': symb[2],
                                  'current_cost': 0, 'current_value': 0}

        current_symb_cost = dict()
        for symb in symb_dict.keys():
            data = lookup(symb)
            symb_dict[data['symbol']]['current_cost'] = data['price']
            symb_dict[data['symbol']]['current_value'] = float(data['price']) * symb_dict[data['symbol']]['amount']

        # print(symb_dict)
        return render_template('index.html', results=symb_dict)

    return apology("TODO")

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock."""
    if request.method == 'GET':
        return render_template('buy.html')
    elif request.method == 'POST':
        if not request.form.get("stock_symb"):
            return render_template('buy_html', error='Forgot to type stock symbol?')
        elif not request.form.get("amount"):
            return render_template('buy_html', error='Forgot to type amount to buy?')

        symb = request.form.get("stock_symb")
        amount = request.form.get("amount")

        try:
            amount = int(amount)
        except:
            return render_template('buy_html', error='Does you enter a valid amount?')

        result = lookup(request.form.get("stock_symb").upper())

        if result:
            price = float(result['price'])
            result = query_db("SELECT cash FROM users WHERE id = ?", int(session["user_id"]), one=True)
            balance = float(result[0])

            withdraw = price * amount

            if withdraw > balance:
                return render_template('buy.html', error='Not enough money available. Your cash: {0};Cash needed: {1}'.format(
                    balance,
                    withdraw
                ))
            else:
                new_balance = balance - withdraw

                try:
                    timestamp = str(datetime.today().timestamp())

                    query_db("UPDATE users SET cash = ? where id = ?", new_balance, int(session["user_id"]), one=True)

                    query_db("INSERT INTO transactions VALUES (NULL, ?, ?, ?, ?, ?, ?, ?)",
                             int(session["user_id"]),
                             symb,
                             amount,
                             price,
                             withdraw,
                             new_balance,
                             timestamp)
                    return render_template('buy.html', info='You bought it!')

                except:
                    render_template('buy.html', error='Error occurred while making a transaction. Try again later.')
        else:
            return render_template('buy.html',
                                   error='Entered stock symbol ({0}) not found'.format(request.form.get("stock_symb")))


    return apology("TODO")

@app.route("/history")
@login_required
def history():
    """Show history of transactions."""
    response = query_db("SELECT symbol, price, amount, timestamp from transactions WHERE user_id = ?", session["user_id"])
    result = []
    for transaction in response:
        symbol, price, amount, timestamp = transaction
        type = 'BUY' if amount > 0 else 'SELL'
        timestamp = datetime.fromtimestamp(float(timestamp))

        transaction_dict = dict()
        transaction_dict['type'] = type
        transaction_dict['symbol'] = symbol
        transaction_dict['price'] = price
        transaction_dict['amount'] = amount
        transaction_dict['date'] = timestamp.strftime("%Y-%m-%d %H:%M:%S")
        result.append(transaction_dict)

    return render_template('history.html', results=result)
    # return apology("TODO")

@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in."""

    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        # query database for username
        rows = query_db("SELECT * FROM users WHERE username = ?", request.form.get("username"), one=True)

        # ensure username exists and password is correct
        if not rows or not pwd_context.verify(request.form.get("password"), rows[2]):
            return apology("invalid username and/or password")

        # remember which user has logged in
        session["user_id"] = rows[0]

        # redirect user to home page
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")

@app.route("/logout")
def logout():
    """Log user out."""

    # forget any user_id
    session.clear()

    # redirect user to login form
    return redirect(url_for("login"))

@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == 'GET':
        return render_template('quote.html')
    elif request.method == 'POST':
        if not request.form.get("stock_symb"):
            return render_template('quote.html', error='Do you forgot to write stock symbol?')

        result = [lookup(request.form.get("stock_symb").upper())]

        if result:
            return render_template('quotes.html', results=result)
        else:
            return render_template('quote.html', error='Entered stock symbol ({0}) not found'.format(request.form.get("stock_symb")))
    return apology("TODO")

@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user."""
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        if not request.form.get("username"):
            return apology("must provide username")

        elif not request.form.get("password"):
            return apology("must provide password")

        elif not request.form.get("password_check"):
            return apology("must retype password")

        pasw = request.form.get("password")
        pasw_ret = request.form.get("password_check")
        usr = request.form.get("username")

        rows = query_db("SELECT * FROM users WHERE username = ?", usr, one=True)

        if pasw != pasw_ret:
            return apology("password you type didn't match. Try again.")
        elif rows:
            return apology("this username already exists")
        else:
            # INSERT INTO 'registrants' (name, dorm) VALUES('David', 'Matthews')
            p_hash = pwd_context.encrypt(pasw)
            result = query_db("INSERT INTO users VALUES (NULL, ?, ?, 100000)", str(usr), str(p_hash), one=True)
            return redirect(url_for("login"))

    else:
        return render_template("register.html")

    return apology("TODO")

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock."""
    response = query_db("SELECT symbol, SUM(amount), SUM(total) from transactions GROUP BY symbol")
    if response:
        symb_dict = dict()
        for symb in response:
            symb_dict[symb[0]] = int(symb[1])

        current_stocks = []
        for symb in symb_dict.keys():
            data = lookup(symb)
            if data:
                current_stocks.append(data)

    if request.method == 'POST':

        if not request.form.get("sell_amount"):
            return render_template('sell.html', error='Need to specify sell amount',
                                   current_stocks=current_stocks,
                                   options=symb_dict)
        if not request.form.get("symb"):
            return render_template('sell.html', error='Need to chose shares symbol',
                                   current_stocks=current_stocks,
                                   options=symb_dict)

        try:
            sell_amount = int(request.form.get("sell_amount"))
        except:
            return render_template('sell.html', error='Only digits available in amount field',
                                   current_stocks=current_stocks,
                                   options=symb_dict)
        sell_symbol = request.form.get("symb")

        if sell_symbol not in symb_dict:
            return render_template('sell.html', error='Unknown share symbol',
                                   current_stocks=current_stocks,
                                   options=symb_dict)

        if sell_amount > symb_dict[sell_symbol] or int(sell_amount) < 0:
            return render_template('sell.html', error='Cant sell more than you have',
                                   current_stocks=current_stocks,
                                   options=symb_dict)
        else:
            new_amount = symb_dict[sell_symbol] - sell_amount
            price = float(lookup(sell_symbol)['price'])
            add_balance = sell_amount * price

            timestamp = str(datetime.today().timestamp())
            balance = query_db("SELECT cash FROM users WHERE id = ?", session["user_id"], one=True)[0]
            new_balance = balance + add_balance

            query_db("UPDATE users SET cash = ? where id = ?", new_balance, int(session["user_id"]), one=True)

            query_db("INSERT INTO transactions VALUES (NULL, ?, ?, ?, ?, ?, ?, ?)",
                     int(session["user_id"]),
                     sell_symbol,
                     sell_amount * -1,
                     price,
                     add_balance,
                     new_balance,
                     timestamp)
            return redirect('sell')
    elif request.method == 'GET':
        return render_template('sell.html', current_stocks=current_stocks, options=symb_dict)
    return apology("TODO")

if __name__ == '__main__':
    app.run(debug = True)