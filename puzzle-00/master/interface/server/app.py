import uuid

from flask import Flask, jsonify, request
from flask_cors import CORS

import os, sys, json
sys.path.append(os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))))

from database.db import *
from controller.lib.constants import *

def conv2JFE(p_dict):
    t_dict = dict((y,x) for x,y in p_dict.items())
    t2_dict = {}
    for (key, value) in t_dict.items():
        # Check if item satisfies the given condition then add to new dict
        if isinstance(key, int):
            t2_dict[key] = value
    return t2_dict


# configuration
DEBUG = True

# instantiate the app
app = Flask(__name__)
app.config.from_object(__name__)

# enable CORS
CORS(app, resources={r'/*': {'origins': '*'}})

# sanity check route
@app.route('/ping', methods=['GET'])
def ping_pong():
    return jsonify('pong!')

@app.route('/books', methods=['GET', 'POST'])
def all_books():
    response_object = {'status': 'success'}
    if request.method == 'POST':
        post_data = request.get_json()
        book = Book(title=post_data.get['title'], author=post_data['author'], read=post_data.get('read'))
        book.save()
        response_object['message'] = 'Book added!'
    else:
        books = []
        for book in Book.select().dicts():
            books.append(book)
        response_object['books'] = books
    return jsonify(response_object)


@app.route('/books/<book_id>', methods=['PUT', 'DELETE'])
def single_book(book_id):
    response_object = {'status': 'success'}
    if request.method == 'PUT':
        post_data = request.get_json()
        book = Book.get_by_id(book_id)
        book.title = post_data.get('title')
        book.author = post_data.get('author')
        book.read = post_data.get('read')
        book.save()
        response_object['message'] = 'Book updated!'
    if request.method == 'DELETE':
        Book.delete_by_id(book_id)
        response_object['message'] = 'Book removed!'
    return jsonify(response_object)

@app.route('/puzzles', methods=['GET', 'POST'])
def all_puzzles():
    response_object = {'status': 'success'}
    if request.method == 'POST':
        post_data = request.get_json()
        # puzzle = Puzzle()
        # puzzle.save()
        response_object['message'] = 'Puzzle added!'
    else:
        puzzles = []
        for puzzle in Puzzle.select().dicts():
            puzzle['state'] = json.loads(puzzle['state'])
            puzzles.append(puzzle)
        response_object['puzzles'] = puzzles
        response_object['state_names'] = conv2JFE(STATE.__dict__)
    return jsonify(response_object)

@app.route('/puzzles/<int:id>', methods=['GET', 'POST'])
def get_puzzle(id):
    print(id)
    response_object = {'status': 'success'}
    if request.method == 'POST':
        post_data = request.get_json()
        # puzzle = Puzzle()
        # puzzle.save()
        response_object['message'] = 'Puzzle added!'
    else:
        puzzles = Puzzle.select().where(Puzzle.id == id).dicts()
        puzzle = puzzles[0]
        puzzle['state'] = json.loads(puzzle['state'])
        response_object['puzzle'] = puzzle
        response_object['state_names'] = conv2JFE(STATE.__dict__)
    return jsonify(response_object)

if __name__ == '__main__':
    app.run()