# Problem set 6

* [Mario (more comfortable)] (http://docs.cs50.net/problems/mario/more/mario.html)
* [Credit (more comfortable)] (http://docs.cs50.net/problems/credit/credit.html)
* [Crack (more comfortable)] (http://docs.cs50.net/problems/crack/crack.html)
* [Sentiments] (http://docs.cs50.net/problems/sentiments/sentiments.html)

# Notes

**Sentiments** 

In order to run application.py localy from IDE (PyCharm e.g), i add to following code:

```python
if __name__ == '__main__':
    app.run(debug = True)
```
Not to use command line for script start i hardcode tweeters API KEY & SECRET in helpers.py. and comment enviroment variables:

```python
API_KEY = ''
API_SECRET = ''

...

# if not os.environ.get("API_KEY"):
#     raise RuntimeError("API_KEY not set")
# if not os.environ.get("API_SECRET"):
#     raise RuntimeError("API_SECRET not set")
```

If you want to run it from CS50 IDE - comment my code and follow instractions given in [Sentiments] (http://docs.cs50.net/problems/sentiments/sentiments.html) part "Getting started".

# Links to read

* [Python basic data types] (http://www.diveintopython3.net/native-datatypes.html)
* [NLTK library HOWTO] (http://www.nltk.org/howto/)
* [Basic sentiment analysis with NLTK] (http://fjavieralba.com/basic-sentiment-analysis-with-python.html)
* [Flask tutorial] (http://flask.pocoo.org/docs/0.11/tutorial/)
