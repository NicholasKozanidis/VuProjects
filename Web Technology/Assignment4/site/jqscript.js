$(function(){               //function to load all code after the HTML is on the DOM
   
    
    $('#table-offer').tablesorter();  //plugin to sort second table
    
  
    
  

$.ajax("http://localhost:8080/phones", { //ajax call to the API with our key
    dataType: "json",
    method: 'GET',
    success: function(response)               //on success, the table is created dynamically
    {
        var tr;
        
        
        for(var i = 0; i<response.length; i++){
            tr = $('<tr/>');
            
            
            tr.append("<td>" +'<input type="text" id="brandp" name="brand">'+ response[i].brand + "</td");
            tr.append("<td>" +'<input type="text" id="modelp" name="model">'+ response[i].model + "</td");
            tr.append("<td>" +'<input type="text" id="osp" name="os">'+ response[i].os + "</td");
            tr.append("<td>" +'<input type="text" id="screensizep" name="screensize">'+ response[i].screensize + "</td");
            tr.append("<td>" +'<input type="text" id="imagep" name="image">'+ "<img src=" + response[i].image +" width=200 height=50 " + ">" + "</td"+"</form>");
            tr.append("<button type=button class=upd_button>Update</button>");
       
           
            $('#table-product tbody').append(tr); //append to body
            
            

        }
        
        
        $('#table-product tfoot').append('<tr><td>Brand<br><input type="text" placeholder="Enter Brand" id="brand" name="brand"></td><td>Model<br><input type="text" placeholder="Enter Model" id="model" name="model"></td><td>OS<br><input type="text" placeholder="Enter Os" id="os" name="os"></td><td>Screen Size<br><input type="text" placeholder="Enter Screen"  id="screensize" name="screensize"></td><td>Image<br><input type="url" id="image" placeholder="Enter Link"  name="image"></td><td><input type="submit" value="Submit" class="sub_button"><button type="reset" value="Reset" class="res_button">Reset</button></td></tr>'); //append the input row
        
        },
        error: function(request, errorType, errorMsg){
            alert("there was an issue with the ajax call: " + errorMsg);
        $('#table-product tfoot').append('<tr><td>Brand<br><input type="text" placeholder="Enter Brand" id="brand" name="brand"></td><td>Model<br><input type="text" placeholder="Enter Model" id="model" name="model"></td><td>OS<br><input type="text" placeholder="Enter Os" id="os" name="os"></td><td>Screen Size<br><input type="text" placeholder="Enter Screen"  id="screensize" name="screensize"></td><td>Image<br><input type="url" id="image" placeholder="Enter Link"  name="image"></td><td><input type="submit" value="Submit" class="sub_button"><button type="reset" value="Reset" class="res_button">Reset</button></td></tr>');
        },
        complete:function(){
           
       $("#table-product").tablesorter();
         
        //call sorter after the function is complete
        
        }
    });
   


    
  
    
    $(document).on("click",".res_button",function(){ //on click, send get call to reset the table and send a second get call to 
        
            $.get( "http://localhost:8080/phones/reset", function() {
            
                alert( "Reseted Database!" );
    $('#table-product tbody tr').remove();
    $("#table-product").trigger("update"); 
                
    })
 });
    
    
    $("#main-content form").on("click",".upd_button",function(){ //on click to update, send put call to the given url, while i managed to return success, the actual data was not changed on the api
        
    var currentrow= $(this).parent();
    var pBrand = $("#brandp").val();
    var pModel = $("#modelp").val();
    var pOs = $("#osp").val();
    var pScreensize =$ ("#screensizep").val();
    var pImage = $("#imagep").val();
    var pDat = {"brand":pBrand, "model":pModel, "os":pOs, "screensize":pScreensize, "image":pImage};
    
        
        var url ='http://localhost:8080/phones/'+pModel;
    $.ajax(url, {
    type: 'PUT',
    dataType: "json",
    data: pDat,
    success: function(result){
             alert("Updated!");
            currentrow.remove();
        
                var tr = $('<tr/>');
           
            tr.append("<td>" + pBrand + "</td");
            tr.append("<td>" + pModel + "</td");
            tr.append("<td>" + pOs + "</td");
            tr.append("<td>" + pScreensize + "</td");
            tr.append("<td>" + "<img src=" + pImage +" width=200 height=50 " + ">" + "</td");
            
            $('#table-product tbody').before(tr);
        
            var resort = true;
            $("#table-product").trigger("update", [resort]);
                alert(JSON.stringify(pDat));
          
        },
    error:function(request,errorType,errorMsg){
            alert("Update unsuccesful: " + errorMsg);
            alert(JSON.stringify(pDat));
            
           
        }
   
        });
    
            
 });
    
    
    
 
     
$("#main-content form").submit(function(e){ //similarly structured ajax call for post, we prevent reloading and then input the new row with the values taken from (this)
    e.preventDefault();
    var formdata = $(this);
    var brand = $(this).find("#brand").val();
    var model = $(this).find("#model").val();
    var os = $(this).find("#os").val();
    var screensize =$(this).find("#screensize").val();
    var image = $(this).find("#image").val();
    
    $.ajax("http://localhost:8080/phones", {
    dataType: "json",
    method: 'POST',
    data: formdata.serialize(),
    success: function(jdata){
             alert("Submitted!");
          
            tr = $('<tr/>');
            
            
            tr.append("<td>" +'<input type="text" id="brandp" name="brand">'+ brand + "</td");
            tr.append("<td>" +'<input type="text" id="modelp" name="model">'+ model + "</td");
            tr.append("<td>" +'<input type="text" id="osp" name="os">'+ os + "</td");
            tr.append("<td>" +'<input type="text" id="screensizep" name="screensize">'+ screensize + "</td");
            tr.append("<td>" +'<input type="text" id="imagep" name="image">'+ "<img src=" + image +" width=200 height=50 " + ">" + "</td"+"</form>");
            tr.append("<button type=button class=upd_button>Update</button>");
       
           
            $('#table-product tbody').append(tr); //append to body
        
            var resort = true;
            $("#table-product").trigger("update", [resort]); //update tablesorter after new row input
        },
    error:function(request,errorType,errorMsg){
            alert("Submit unsuccesful: " + errorMsg);
        },
    complete:function(){
        $('input[type="text"],textarea').val(''); //empty the form text
        
            }
   
        });
    
    });
    
});

