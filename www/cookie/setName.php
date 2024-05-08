<?php
if (isset($_POST["name"])) {
	setcookie('name', $_POST["name"], time() + 120, "/");
}
?>
<!doctype html>
<html>

<head>
	<title>PHP</title>
	<link rel="stylesheet" href="https://cdn.jsdelivr.net/npm/bootstrap@5.3.3/dist/css/bootstrap.min.css">
	<link rel="preconnect" href="https://fonts.googleapis.com">
	<link rel="preconnect" href="https://fonts.gstatic.com" crossorigin>
	<link href="https://fonts.googleapis.com/css2?family=Roboto+Mono:ital,wght@0,100..700;1,100..700&display=swap"
		rel="stylesheet">
</head>
<body class="m-3 mt-5" style="background-color:#e5e5e5">
	<div class="container p-3">
		<div class="rounded-3 p-5" style="background-color:#fcfcfc; border:solid 1px #cdcdcd">
			<center>
				<h2>Bonjour
		<?php
		if (isset($_POST['name'])) {
			echo $_POST["name"];
		} else if(isset($_COOKIE['name'])){
			echo $_COOKIE['name'];
		} else {
			echo 'inconnu';
		}
		?>
				</h2>
			</center>
		</div>
	</div>
</body>
</html>
