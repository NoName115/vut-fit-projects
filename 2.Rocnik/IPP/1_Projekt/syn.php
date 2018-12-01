<?php

require('argParser.lib.php');
require('parser.lib.php');
require('syntaxHighlight.lib.php');


$arg = new ArgumentParserAndFiles($argv);
$parser = new CommandParser();

// Parse arguments
if (($errorCatch = $arg->parseIt()) != 0)
{
	exit($errorCatch);
}

// End program if --help parameter is set
if ($arg->getHelp())
{
	exit(0);
}

// Check, parse and close format file
while ($output = $arg->getFormatLine())
{
	$errorCatch = $parser->addFormatString($output);

	// Error in format file
	if ($errorCatch == FORMAT_FILE_ERROR)
	{
		$arg->closeFormatFile();
		exit($errorCatch);
	}
}
$arg->closeFormatFile();

// Read input file, or read stdin
$inputData = $arg->getInputFileData();
$arg->closeInputFile();

// Highlight input code
$highlight = new SyntaxHighlight($inputData, $parser->getCommands(), $arg->getBr());
$highlightedCode = $highlight->highlightCode();

// Catch error code
if ($highlightedCode == 4)
{
	$arg->closeOutputFile();
	exit($highlightedCode);
}

// Write highlighted text to outputFile and close it
$arg->printToOutputFile($highlightedCode);
$arg->closeOutputFile();

?>