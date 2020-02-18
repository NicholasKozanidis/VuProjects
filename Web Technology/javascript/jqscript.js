//function to load all code after the HTML is on the DOM
$(function(){  
    //plugin to sort second table
    $('#table-offer').tablesorter();		
    
	//ajax call to the API with our key
	$.ajax("https://wt.ops.labs.vu.nl/api18/adf12fb0", {
		dataType: "json",
		method: 'GET',
		//on success, the table is created dynamically
		success: function(response){
			var tr;
        
			for(var i = 0; i<response.length; i++){
				tr = $('<tr/>');
				tr.append("<td>" + response[i].brand + "</td");
				tr.append("<td>" + response[i].model + "</td");
				tr.append("<td>" + response[i].os + "</td");
				tr.append("<td>" + response[i].screensize + "</td");
				tr.append("<td>" + "<img src=" + response[i].image +" width=200 height=50 " + ">" + "</td");
				//append to body
				$('#table-product tbody').append(tr);
			}
			//append the input row
			$('#table-product tfoot').append('<tr><td>Brand<br><input type="text" placeholder="Enter Brand" id="brand" name="brand"></td><td>Model<br><input type="text" placeholder="Enter Model" id="model" name="model"></td><td>OS<br><input type="text" placeholder="Enter Os" id="os" name="os"></td><td>Screen Size<br><input type="text" placeholder="Enter Screen"  id="screensize" name="screensize"></td><td>Image<br><input type="url" id="image" placeholder="Enter Link"  name="image"></td><td><input type="submit" value="Submit" class="sub_button"><button type="reset" value="Reset" class="res_button">Reset</button></td></tr>');
		},
		error: function(request, errorType, errorMsg){
			alert("there was an issue with the ajax call: " + errorMsg);
		},
    
		//call sorter after the function is complete
		complete:function(){
			$("#table-product").tablesorter();
		}
	});

    //on click, send get call to reset the table and send a second get call to retrieve the reseted data
    $(document).on("click",".res_button",function(){ 
   
        $.get( "https://wt.ops.labs.vu.nl/api18/adf12fb0/reset", function() {
			alert( "Reseted Database!" );
			$('#table-product tbody tr').remove();
			$("#table-product").trigger("update"); //remove table body trs
        });
        
		//send a second get request to fill the table with the reseted data
		$.get("https://wt.ops.labs.vu.nl/api18/adf12fb0", function(data) { 
			var tr;
        
			for(var i = 0; i<data.length; i++){
				tr = $('<tr/>');
            
				tr.append("<td>" + data[i].brand + "</td");
				tr.append("<td>" + data[i].model + "</td");
				tr.append("<td>" + data[i].os + "</td");
				tr.append("<td>" + data[i].screensize + "</td");
				tr.append("<td>" + "<img src=" + data[i].image +" width=200 height=50 " + ">" + "</td");
           
				$('#table-product tbody').append(tr); //append to body
			}
        $("#table-product").trigger("update"); //update table sort plugin
        });
	});
    
  
 
	//similarly structured ajax call for post, we prevent reloading and then input the new row with the values taken from (this)     
	$("#main-content form").submit(function(e){ 
		e.preventDefault();
		var formdata = $(this);
		var brand = $(this).find("#brand").val();
		var model = $(this).find("#model").val();
		var os = $(this).find("#os").val();
		var screensize =$(this).find("#screensize").val();
		var image = $(this).find("#image").val();
    
		$.ajax("https://wt.ops.labs.vu.nl/api18/adf12fb0", {
			dataType: "json",
			method: 'POST',
			data: formdata.serialize(),
			success: function(jdata){
            
				alert("Submitted!");
           
				var tr = $('<tr/>');
			
				tr.append("<td>" + brand + "</td");
				tr.append("<td>" + model + "</td");
				tr.append("<td>" + os + "</td");
				tr.append("<td>" + screensize + "</td");
				tr.append("<td>" + "<img src=" + image +" width=200 height=50 " + ">" + "</td");
            
				$('#table-product').find("tbody tr:last").after(tr);
			
				var resort = true;
				//update tablesorter after new row input
				$("#table-product").trigger("update", [resort]); 
			},
		error:function(request,errorType,errorMsg){
			alert("Submit unsuccesful: " + errorMsg);
        },
		//empty the form text on completion
		complete:function(){
			$('input[type="text"],textarea').val(''); 
        }
		});    
    });
});

