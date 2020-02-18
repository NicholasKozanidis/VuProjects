from bottle import response, error, get, request, post, delete, put
import json


###############################################################################
#                                   Routes                                    #
###############################################################################

#GET request to get a list of all phones from database
@get('/api')
def getPhones(db):
	try:
		db.execute("SELECT * FROM phones")

		phones = db.fetchall()

		# TODO: add code that checks for errors so you know what went wrong
		response.status = 200
		response.headers['Content-Type'] = 'application/json'
		response.headers['Cache-Control'] = 'no-cache'	
		return json.dumps(phones)
	except:
		response.status = 500
		return "error: Cant get the data!"
		
#GET request to get a certain phone from database
@get('/api/<id>')	
def getPhoneById(db, id):
	try:
		db.execute("SELECT * FROM phones where id=?",(id,))
	
		phones = db.fetchone()
	
		response.headers['Content-Type'] = 'application/json'
		response.headers['Cache-Control'] = 'no-cache'	
		return json.dumps(phones)
	except:
		response.status = 500
		return "error: cant get phone"

#post request for posting a new entry		
@post('/api')
def addPhone(db):
	try:
		db.execute("INSERT INTO phones (brand, model, os, image, screensize) VALUES (?, ?, ?, ?, ?)",
		(request.json['brand'],
		request.json['model'],
		request.json['os'],
		request.json['image'],
		request.json['screensize']))
		response.status = 200
		response.headers['Content-Type'] = 'application/json'
		response.headers['Cache-Control'] = 'no-cache'
		return "Success: Post succesful!"
	except:
		response.status = 500
		return "error: post failed"
		
#PUT request for updating a certain phone in database
@put('/api/<id>')
def updatePhone(db, id,):
	try:
		db.execute("UPDATE phones SET brand=?, model=?, os=?, image=?, screensize=? WHERE id=?",
			(request.json['brand'],
			request.json['model'], 
			request.json['os'],
			request.json['image'], 
			request.json['screensize'], 
			id))
		response.status = 200
		response.headers['Content-Type'] = 'application/json'
		response.headers['Cache-Control'] = 'no-cache'
		return "Success: Update succesful!"
	except:
		response.status = 500
		return "error: update failed"

#DELETE request	for emptying database
@delete('/api/reset')
def resetDatabase(db):
	try:
		db.execute("DELETE FROM phones")
		return "Success: Delete succesful!"
	except:
		response.status = 500
		return "Error: cant delete!"


if __name__ == "__main__":
    from bottle import install, run
    from wtplugin import WtDbPlugin, WtCorsPlugin

    install(WtDbPlugin())
    install(WtCorsPlugin())
    run(host='localhost', port=8080, reloader=True, debug=True, autojson=False)
