<?php

define("FORMAT_FILE_ERROR", 4);


class CommandParser
{
	/**
	 * All known commands
	 * @var array
	 */
	private $commands = ['bold', 'italic', 'underline', 'teletype', 'size', 'color'];

	/**
	 * Commands from formatFile
	 * @var array
	 */
	private $allCommands = array();

	/**
	 * Check and parse one line from formatFile
	 * @param string $inputFormatLine One line from formatFile
	 * @return integer Error code
	 */
	public function addFormatString($inputFormatLine)
	{
		preg_match('(^([\S ]+)\t+([\S\t ]+)$)', $inputFormatLine, $match);
		if (count($match) == 0)
		{
			return FORMAT_FILE_ERROR;
		}

		// Convert input regex to original regex
		$regex = $this->getOriginalRegex($match[1]);

		// Check input commands
		$checkedCommands = $this->checkCommands($match[2]);

		// Add regex and commands to array
		if ($regex != FORMAT_FILE_ERROR && $checkedCommands != FORMAT_FILE_ERROR)
		{
			$this->allCommands[] = array('regex'=>$regex ,'commands'=>$checkedCommands);
			return 0;
		}
		else
		{
			return FORMAT_FILE_ERROR;
		}
	}

	/**
	 * Function convert pseudo regex to original regex
	 * @param string $inputRegex Input pseudo-regex
	 * @return string $inputRegex Converted regex
	 */
	private function getOriginalRegex($inputRegex)
	{
		// Catch error regex
		preg_match_all('(%\\\)', $inputRegex, $match);
		if (count($match[0]) != 0)
		{
			return FORMAT_FILE_ERROR;
		}

		// Catch error regex
		preg_match_all('(\.\.+)', $inputRegex, $match);
		if (count($match[0]) != 0)
		{
			return FORMAT_FILE_ERROR;
		}

		// Catch error regex
		preg_match_all('((?<!%)\!\!+)', $inputRegex, $match);
		if (count($match[0]) != 0)
		{
			return FORMAT_FILE_ERROR;
		}

		// Replace ^_\[]${},? 	-> (\^|\_|\\|\[|\]|\$|\{|\}|\,|\?)
		$inputRegex = preg_replace('((\^|\_|\\\|\[|\]|\$|\{|\}|\,|\?))', '\\\\\1', $inputRegex);
		// !A 				-> 		[^A]
		$inputRegex = preg_replace('(!([%|\\\]?.))', '[^\1]', $inputRegex);
		// A.B 				-> 		AB
		$inputRegex = preg_replace('((?<!%)\.)', '\1', $inputRegex);
		// AB resp. A.B.	-> 		OK
		// A | B 			-> 		OK
		// A*				-> 		OK
		// A+				-> 		OK
		// (A)				-> 		OK
		// %s 				-> 		\s
		$inputRegex = preg_replace('(%s)', '\s', $inputRegex);
		// %a 				-> 		.
		$inputRegex = preg_replace('(%a)', '.', $inputRegex);
		// %d 				-> 		[0-9]
		$inputRegex = preg_replace('(%d)', '[0-9]', $inputRegex);
		// %l 				-> 		[a-z]
		$inputRegex = preg_replace('(%l)', '[a-z]', $inputRegex);
		// %L 				-> 		[A-Z]
		$inputRegex = preg_replace('(%L)', '[A-Z]', $inputRegex);
		// %w 				-> 		[a-zA-Z]
		$inputRegex = preg_replace('(%w)', '[a-zA-Z]', $inputRegex);
		// %W 				-> 		[a-zA-Z0-9]
		$inputRegex = preg_replace('(%W)', '[a-zA-Z0-9]', $inputRegex);
		// %t 				-> 		\\t
		$inputRegex = preg_replace('(%t)', '\\t', $inputRegex);
		// %n 				-> 		\\n
		$inputRegex = preg_replace('(%n)', '\\n', $inputRegex);
		// %(.|!*+()%)		-> 		%(\.|\||\!|\*|\+|\(|\)|\%)
		$inputRegex = preg_replace('(%(\.|\||\!|\*|\+|\(|\)|\%))', '\\\\\1', $inputRegex);

		// Catch error regex
		preg_match_all('((\[\^\(\])|(\[\^\)\]))', $inputRegex, $match);
		if (count($match[0]) != 0)
		{
			return FORMAT_FILE_ERROR;
		}

		return $inputRegex;
	}

	/**
	 * Function check and parse rules from formatFile
	 * @param array $inputCommands Input array of rules
	 * @return array $outputCommands Output array of rules
	 */
	private function checkCommands($inputCommands)
	{
		$splitedCommands = preg_split('([\t ]*(,)[\t ]*)', $inputCommands);
		$outputCommands = array();

		for ($i = 0; $i < count($splitedCommands); $i++)
		{
			$splited = explode(':', $splitedCommands[$i]);

			if (!in_array($splited[0], $this->commands))
			{
				return FORMAT_FILE_ERROR;
			}

			if ($splited[0] == 'size')
			{
				if ($splited[1] <= 0 || $splited[1] > 7)
				{
					return FORMAT_FILE_ERROR;
				}
			}
			elseif ($splited[0] == 'color')
			{
				if(!(ctype_xdigit($splited[1])))
				{
					return FORMAT_FILE_ERROR;
				}
			}

			$outputCommands[] = $splited;
		}

		return $outputCommands;
	}

	/**
	 * Function simple return all commands
	 * @return array $this->allCommands All commands from formatFile
	 */
	public function getCommands()
	{
		return $this->allCommands;
	}
}


?>