 $("#login-button").click(function(event){
		 event.preventDefault();
	 
	 $('form').fadeOut(500);
	 $('.wrapper').addClass('form-success');
});

function password_md5_formSubmit()
  {
  	//alert(hex_md5(document.getElementById("login_password").value);
  	document.getElementById("login_password").value = hex_md5(document.getElementById("login_password").value);
  	document.getElementById("submit_form").submit();
  };