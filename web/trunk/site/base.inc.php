<?php

	require_once 'html.inc.php';


	$MENU_OPTIONS = array(
		'home' => array(
			'link' => 'index.php',
			'text' => 'Home',
			'tooltip' => 'Main page'
		),
		'download' => array(
			'link' => 'download.php',
			'text' => 'Download',
			'tooltip' => 'Download released packages or access the subversion repository'
		),
		'screenshots' => array(
			'link' => 'screenshots.php',
			'text' => 'Screenshots',
			'tooltip' => 'See jMList in action'
		),
		'docs' => array(
			'link' => 'docs.php',
			'text' => 'Documentation',
			'tooltip' => 'Read user or developer documentation'
		),
		'ml' => array(
			'link' => 'ml.php',
			'text' => 'Mailing Lists',
			'tooltip' => 'Talk about jMList development or get support'
		),
		'berlios' => array(
			'link' => 'http://developer.berlios.de/projects/jmlist/',
			'text' => 'Project Page',
			'tooltip' => 'Visit the project page at berlios.de'
		)
	);


	function topheading( $current_id, $title = null )
	{
		global $MENU_OPTIONS;

		open( 'table', array( 'id' => 'topheading' ) );

		tag( 'tr', array( 'id' => 'r1' ),
			txttag( 'td', array( 'id' => 'c11' ), ' ' ).
			txttag( 'td', array( 'id' => 'c12' ), ' ' )
		);

		open( 'tr', array( 'id' => 'r2' ) );

		tag( 'td', array( 'id' => 'c21' ), ' ');

		open( 'td', array( 'id' => 'menu' ) );

		$i = 0;
		foreach ( $MENU_OPTIONS as $t_id => $t_link ) {
			if ( $i++ != 0 )
				echo ' - ';

			tag( 'a',
				array(
					'class' => ( $t_id == $current_id ? 'current' : null ),
					'href' => $t_link['link'],
					'title' => $t_link['tooltip']
				),
				escape ( $t_link['text'] )
			);
		}

		close( 'table' );
	}

?>
