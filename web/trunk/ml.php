<?php

	require_once 'site/base.inc.php';

	head( 'Mailing Lists - jMList' );
	topheading( 'ml' );


	open( 'div', array( 'id' => 'body' ) );

	h2( 'General Mailing List' );

	$_jMList = txttag( 'span', array( 'class' => 'jmlist' ), 'jMList' );

	tag( 'p', array(),
		"While $_jMList is a small project, a single mailing list is more ".
		"than enough. In the future, a separate list may be created just for ".
		"development talk."
	);

	tag( 'p', array(),
		"You can join the user's list by visiting the list page at ".
		a( 'http://lists.berlios.de/mailman/listinfo/jmlist-users' ). "."
	);

?>
