from flask import Flask
from flask import render_template, request, redirect, url_for
from db import User
app = Flask(__name__)

@app.route('/')
def index():
    users = User.select()
    return render_template('index.html', users=users)

@app.route('/user/<string:username>')
def show_user_profile(username):
    user = User.get_or_none(User.username == username)
    return 'User %s' % user.username if user else 'User not found'

@app.route('/user', methods=['POST'])
def create_user():
    if (not request.form['username']):
        return 'Username is required'
    user = User(username = request.form['username'])
    user.save()
    return redirect(url_for('index'))

@app.route('/about')
def about():
    return 'About Page'

@app.errorhandler(404)
def not_found(error):
    return '404 Not Found', 404

if __name__ == "__main__":
   app.run(host='127.0.0.1', port=5000, debug=True)