<?php

	require_once 'Net_UserAgent/Detect.php';

	# Headers to prevent caching
	header( 'Pragma: no-cache' );
	header( 'Expires: Mon, 25 Feb 2002 11:00:00 GMT' );
	header( 'Cache-Control: no-store, no-cache, must-revalidate' );
	header( 'Cache-Control: post-check=0, pre-check=0', false );

	$HTML_CHARSET = 'UTF-8';
	$HTML_OPEN_TAGS = array();

	register_shutdown_function( 'html_finalize' );


	function escape( $string ) {
		global $HTML_CHARSET;
		return htmlentities( $string, ENT_COMPAT, $HTML_CHARSET );
	}


	function html_finalize() {
		global $HTML_OPEN_TAGS;

		close_until( 'body' );
		tag( 'hr' );
		tag( 'div',
			array(
				'id' => 'footer'
			),
			txttag( 'a',
				array(
					'href' => 'http://developer.berlios.de',
					'title' => 'BerliOS Developer'
				),
				txttag( 'img',
					array(
						'src' => 'http://developer.berlios.de/bslogo.php?group_id=3105',
						'width' => '124px',
						'height' => '32px',
						'border' => '0',
						'alt' => 'BerliOS Developer Logo'
					)
				)
			)
		);

		close( 'html' );
	}


	function txttag( $name, $attributes = array(), $content = null ) {
		global $HTML_CHARSET;

		$tag = $name;

		foreach ( $attributes as $key => $val )
			if ( $val !== null )
				$tag .= ' ' . $key . '="' .
					htmlentities( $val, ENT_COMPAT, $HTML_CHARSET ) . '"';

		if ( isset( $content ) )
			return "<{$tag}>$content</{$name}>";
		else
			return "<{$tag} />";
	}


	function tag( $name, $attributes = array(), $content = null ) {
		echo txttag( $name, $attributes, $content ) . "\r\n";
	}


	function open( $name, $attributes = array() ) {
		global $HTML_CHARSET, $HTML_OPEN_TAGS;

		$tag = $name;

		foreach ( $attributes as $key => $val )
			if ( $val !== null )
				$tag .= ' ' . $key . '="' .
					htmlentities( $val, ENT_COMPAT, $HTML_CHARSET ) . '"';

		echo "<{$tag}>";
		array_push( $HTML_OPEN_TAGS, $name );
	}


	function close( $name = null ) {
		global $HTML_OPEN_TAGS;

		if ( isset( $name ) ) {
			do {
				if ( ( $tag = array_pop( $HTML_OPEN_TAGS ) ) )
					echo "</{$tag}>\r\n";
			}
			while ( $tag && $tag != $name );
		}
		else {
			$tag = array_pop( $HTML_OPEN_TAGS );
			echo "</{$tag}>\r\n";
		}
	}


	function close_until( $name ) {
		global $HTML_OPEN_TAGS;

		while ( ( $tag = array_pop( $HTML_OPEN_TAGS ) ) ) {
			if ( $tag == $name ) {
				array_push( $HTML_OPEN_TAGS, $tag );
				return;
			}

			echo "</{$tag}>";
		}
	}


	function head( $title = null, $body_class = null ) {
		global $HTML_CHARSET;
		static $st = false;

		if ( $st ) return;
		$st = true;

		if ( Net_UserAgent_Detect::isIE() )
			header( 'Content-type: text/html; charset=' . $HTML_CHARSET );
		else
			header( 'Content-type: application/xhtml+xml; charset=' . $HTML_CHARSET );

		echo '<?xml version="1.0" encoding="' . $HTML_CHARSET . '" ?>' . "\r\n";
		echo '<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">' . "\r\n";
		#echo '<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.1//EN" "http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd">' . "\r\n";

		open( 'html', array(
			'xmlns'			=> 'http://www.w3.org/1999/xhtml',
			'xml:lang'		=> 'en'
		) );
		open( 'head' );
		tag( 'meta', array(
			'http-equiv'	=> 'Content-Type',
			'content'		=> 'application/xhtml+xml',
			'charset'		=> $HTML_CHARSET
		) );
		tag( 'meta', array(
			'name'			=> 'Author',
			'content'		=> 'Juliano Ravasi Ferraz'
		) );
		tag( 'link', array(
			'rel'			=> 'stylesheet',
			'type'			=> 'text/css',
			'href'			=> 'default.css'
		) );

		if ( isset( $title ) )
			tag( 'title', array(), $title );

		close();	// head

		open( 'body', array( 'class' => $body_class ) );
	}


	function h1( $string, $attributes = array() ) {
		tag( 'h1', $attributes, $string );
	}


	function h2( $string, $attributes = array() ) {
		tag( 'h2', $attributes, $string );
	}


	function h3( $string, $attributes = array() ) {
		tag( 'h3', $attributes, $string );
	}


	function a( $content, $link = null, $tooltip = null ) {
		if ( is_null( $link ) )
			$link = $content;

		return txttag( 'a',
			array(
				'href' => $link,
				'title' => $tooltip
			),
			$content
		);
	}


	function acronym( $acronym, $title ) {
		return txttag( 'acronym', array( 'title' => $title ), escape( $acronym ) );
	}


	function href( $page, $fields = null ) {
		if ( is_array( $fields ) ) {
			$t_fields = array();
			foreach ( $fields as $k => $v )
				$t_fields[] = $k . '=' . urlencode( $v );

			$page .= '?' . join( '&', $t_fields );
		}

		return $page;
	}


	function form_button( $text, $img = null, $name = null,
						  $value = '1', $class = null, $type = 'submit' ) {
		$attributes['type'] = $type;

		if ( $class !== null )
			$attributes['class'] = $class;

		if ( Net_UserAgent_Detect::isIE() ) {
			# Hack to work around IE bug with form buttons. IE has really, really
			# stupid handling of <button> tags, which makes them unusable. On the
			# other side, <input> tags reserves the value= attribute for the text
			# that appers inside the button, so we have to encode both name and
			# value in the name of the button.
			if ( $name !== null )
				$attributes['name'] = $name . '_v[' . $value . ']';

			$attributes['value'] = $text;

			return txttag( 'input', $attributes );
		}
		else {
			# Here we use the new <button> tag, which is much more powerful and
			# is compatible with 
			if ( $name !== null ) {
				$attributes['name'] = $name;
				$attributes['value'] = $value;
			}

			$contents = "";

			if ( $img !== null )
				$contents .= txttag( 'img',
					array(
						'alt'	=> '',
						'class'	=> 'button-img',
						'src'	=> 'img/' . $img
					)
				);

			$contents .= txttag( 'span',
				array( 'class' => 'button-text' ),
				escape( $text )
			);

			return txttag( 'button', $attributes, $contents );
		}
	}


	function form_hidden( $name, $value = null ) {
		return txttag( 'input',
			array(
				'type' => 'hidden',
				'name' => $name,
				'value' => $value
			)
		);
	}


?>
