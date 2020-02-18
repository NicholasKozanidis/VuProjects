###############################################################################
# Web Technology at VU University Amsterdam
# Assignment 3
#
# The assignment description is available on Canvas.
# This is a template for you to quickly get started with Assignment 3.
# Read through the code and try to understand it.
#
# Have you looked at the documentation of bottle.py?
# http://bottle.readthedocs.org/en/stable/index.html
#
# Once you are familiar with bottle.py and the assignment, start implementing
# an API according to your design by adding routes.
###############################################################################

# Include more methods/decorators as you use them
# See http://bottle.readthedocs.org/en/stable/api.html#bottle.Bottle.route

from bottle import response, error, get, post, put, delete, request, abort
import json

#GET route, retrieves all phones from the database
@get('/phones')
def getAll(db):
    db.execute("SELECT * FROM phones")
    response_body = db.fetchall()  
    if response_body == []:	#If database is empty, abort
    	abort(404, 'The Database Is Empty, add Data First')
    
    return json.dumps(response_body, sort_keys=True, indent=4, separators=(',', ': ')) 



#GET route, finds a specific phone based on model
@get('/phones/<model>')
def getOne(db, model):
	db.execute("""SELECT id, brand, model, os, image, screensize
                    FROM phones WHERE model=?""", (model,))
	response_body = db.fetchall()
	if response_body == []:		#Check if the list is empty
		abort(404, 'Entry doesnt exist')	#If it is, abort
	else:									#If not, set response MIME type and status
		response.content_type = 'application/json'
		response.status = '200 OK'
	return json.dumps(response_body, sort_keys=True, indent=4, separators=(',', ': '))



#PUT route, modifies a phone in the database, all previous fields are required to be input again
@put('/phones/<model>')
def updatePhones(db, model):
	modBrand = request.forms.get('brand') #Retrieve all the form data if the body is sent as form data
	modModel = request.forms.get('model')
	modOs = request.forms.get('os')
	modImage = request.forms.get('image')
	modScreensize = request.forms.get('screensize')
	if modBrand: #If modBrand has a value, add to database
		db.execute("""UPDATE phones SET brand=?, model=?, os=?, image=?, screensize=? WHERE model=?""",
                    (modBrand, modModel, modOs, modImage, modScreensize, model))
	else: #If it doesnt have a value, check for json data
		item = request.json
		if item == []: #If requested json data doesnt exist, abort
			abort(400, 'Entry Modification Failed')
		db.execute("""UPDATE phones SET brand=?, model=?, os=?, image=?, screensize=? WHERE model=?""",
                    (item['brand'], item['model'], item['os'], item['image'], item['screensize'], model))

	db.execute("SELECT * FROM phones")
	response_body = db.fetchall()
	response.content_type = 'application/json' 
	response.status = 204 
	return



#Post route, adds one phone to the database, can be form or json data
@post('/phones')
def createPhone(db):
	#Request all the data from a form
	newBrand = request.forms.get('brand')
	newModel = request.forms.get('model')
	newOs = request.forms.get('os')
	newImage = request.forms.get('image')
	newScreensize = request.forms.get('screensize')
	
	if newBrand: #If the form data holds actual values, add those to the database
		db.execute(""" INSERT INTO phones (brand, model, os, image, screensize)
	                VALUES (?, ?, ?, ?, ?)""",
	                (newBrand, newModel, newOs, newImage, newScreensize))
		db.execute("SELECT * FROM phones")
		response_body = db.fetchall()
	else: #If the form data was empty, check for json in request body
		item = []
		item = request.json
		if item == []: #If no json in request body, or body is empty, abort
			abort(400, 'No data was included, cant be added to database')
	
		db.execute(""" INSERT INTO phones (brand, model, os, image, screensize)
		               VALUES (?, ?, ?, ?, ?)""",
		               (item['brand'], item['model'], item['os'], item['image'], item['screensize']))
			
	db.execute("SELECT * FROM phones")
	response_body = db.fetchall()
	response.content_type = 'application/json' 
	response.status = '201 Created' 
	return json.dumps(response_body, sort_keys=True, indent=4, separators=(',', ': '))



#Delete route, deletes an entry based on model
@delete('/phones/<model>')
def deleteOne(db, model):
	db.execute("""SELECT * FROM phones WHERE model=?""", (model,)) #First check whether the entry exists
	existCheck = db.fetchall()
	if existCheck == []: #If the entry doesnt exist it can be deleted, abort
		abort(404, 'Non-existant data cant be deleted')

	db.execute("DELETE FROM phones WHERE model=?", (model,))
	db.execute("SELECT * FROM phones")
	response_body = db.fetchall()
	response.content_type = 'application/json' 
	response.status = 204  
	return

#Get request	for emptying database
@get('/phones/reset')
def resetDatabase(db):
    db.execute("SELECT * FROM phones")
    check = db.fetchall()
    if check == []: #If the entry doesnt exist it can be deleted, abort
        abort(404, 'Non-existant data cant be deleted')
        
    db.execute("DELETE FROM phones")
    response.status = 204
    return
		



#Custom errors
#Custom error 400 route.
@error(400)
def error400(error):
	response.status = 400
	response.content_type = 'text/html'
	response_body = '<h1>Error: 400</h1><br><h3>A request has been made, the server couldnt understand it, try modifying the request</h3>'
	return response_body

#Custom error404 route.
@error(404) 
def error404(error):
	response.status = 404
	response.content_type = 'text/html'
	response_body = '<h1>Error 404</h1><br><h3>It appears the resource you were trying to access doesnt exist, you can try altering the model field</h3>'
	return response_body

@error(405)
def error405(error):
	response.status = 405
	response.content_type = 'text/html'
	response_body = '<h1>Error 405</h1><br><h3>The method you tried is not allowed on this resource, check API for the methods allowed on this resource</h3>'
	return response_body

@error(500)
def error500(error):
	response.status = 500
	response.content_type = 'text/html'
	response_body = '<h1>Error 500</h1><br><h3>And unexpected server-side error has occured, try again later.</h3>'
	return response_body




###############################################################################
# This starts the server
#
# Access it at http://localhost:8080
#
# If you have problems with the reloader (i.e. your server does not
# automatically reload new code after you save this file), set `reloader=False`
# and reload manually.
#
# You might want to set `debug=True` while developing and/or debugging and to
# `False` before you submit.
#
# The installed plugin 'WtPlugin' takes care of enabling CORS (Cross-Origin
# Resource Sharing; you need this if you use your API from a website) and
# provides you with a database cursor.
###############################################################################

if __name__ == "__main__":
    from bottle import install, run
    from wtplugin import WtDbPlugin, WtCorsPlugin

    install(WtDbPlugin())
    install(WtCorsPlugin())
    run(host='localhost', port=8080, reloader=True, debug=True, autojson=False)
