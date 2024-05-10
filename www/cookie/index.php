<!doctype html>
<html>

<head>
	<title>Cookie</title>
	<link rel="stylesheet" href="https://cdn.jsdelivr.net/npm/bootstrap@5.3.3/dist/css/bootstrap.min.css">
	<link rel="preconnect" href="https://fonts.googleapis.com">
	<link rel="preconnect" href="https://fonts.gstatic.com" crossorigin>
	<link href="https://fonts.googleapis.com/css2?family=Roboto+Mono:ital,wght@0,100..700;1,100..700&display=swap"
		rel="stylesheet">
</head>
	<div class="container p-3">
		<div class="rounded-3 p-5" style="background-color:#fcfcfc; border:solid 1px #cdcdcd">
			<center>
				<h2>Cookie</h2>
				</center>
			<form method="post" action="/bonjour/">
				<div class="mb-3">
					<label for="name" class="form-label">Name</label>
					<input id="name" name="name" type="text" class="form-control" />
					<div id="help" class="form-text">
		<?php
		if(isset($_COOKIE['name'])){
			echo 'Un cookie exist pour '.$_COOKIE['name'].'.';
		} else {
			echo 'Il n\'exist pas de cookie pour le moment...';
		}
		?>
					</div>
				</div>
				<button class="btn btn-primary">Upload</button>
			</form>
			
		</div>
	</div>
</body>
</html>
