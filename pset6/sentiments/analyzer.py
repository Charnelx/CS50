import nltk
from nltk.corpus import stopwords
from nltk.tokenize import TweetTokenizer
from collections import defaultdict
import os

class Analyzer:
    """Implements sentiment analysis."""

    def __init__(self, positives="positive-words.txt", negatives="negative-words.txt"):
        """Initialize Analyzer."""
        path = os.path.dirname(__file__)

        self.pos_file = os.path.join(path, positives)
        self.neg_file = os.path.join(path, negatives)

        # init stop-words list for text cleaning
        self.stop_words = set(stopwords.words('english'))
        # init tokenizer
        self.tknzr = TweetTokenizer(strip_handles=True, reduce_len=True)
        # init words steamer
        self.lancaster = nltk.SnowballStemmer(language='english')

        # loads pos/neg dictionaries into memory
        self._load_ranking()

    def _load_ranking(self):

        # init sets for storing dictionaries
        self.pos_set = set()
        self.neg_set = set()

        with open(self.pos_file, mode='r') as p_file:
            while True:
                word = p_file.readline()
                if not word:
                    break
                if len(word) < 2:
                    pass
                else:
                    word = word.rstrip('\n')
                    self.pos_set.add(word)

        with open(self.neg_file, mode='r') as n_file:
            while True:
                word = n_file.readline()
                if not word:
                    break
                if len(word) < 2:
                    pass
                else:
                    word = word.rstrip('\n')
                    self.neg_set.add(word)

    def analyze(self, text):
        """Analyze text for sentiment, returning its score."""
        # initialize variables
        freq = defaultdict(lambda: 0)
        punctuation = '!"#$%&\'()*+,-./:;<=>?@[\\]^_`{|}~'

        for tweet in text:
            # split text into tokens
            tokens = self.tknzr.tokenize(tweet)

            # normalize text
            normalized_step = [word.lower() for word in tokens if word.lower() not in self.stop_words]
            # normalized_step = [self.lancaster.stem(word.lower()) for word in tokens if word.lower() not in self.stop_words]

            # clean text
            normalized = [word for word in normalized_step if word not in punctuation and word.isalnum()]

            # feel word frequency dictionary
            for word in normalized:
                freq[word] += 1

        positive, negative, neutral = 0.0, 0.0, 0.0

        # process words frequencies
        for k, v in freq.items():
            if k in self.pos_set:
                positive += v
            elif k in self.neg_set:
                negative += v
            else:
                neutral += v

        return positive, negative, neutral

    # alter analyze method to return total score of given text
    def getScore(self, text):
        positive, negative, neutral = self.analyze(text)
        results = {
            'positive': positive,
            'negative': negative,
            'neutral': neutral,
        }

        max_score = max(results, key=lambda i: results[i])

        if max_score == 'positive':
            return results[max_score]
        elif max_score == 'negative':
            return results[max_score] * -1
        else:
            return 0.0