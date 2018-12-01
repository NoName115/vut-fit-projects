<?php


class SyntaxHighlight
{
	/**
	 * Input text data
	 * @var string
	 */
	private $input;

	/**
	 * All commands from formatFile
	 * @var array
	 */
	private $commands;

	/**
	 * --br parameter
	 * @var boolean
	 */
	private $isBr;

	/**
	 * All begin tags
	 * key - index in inputFile
	 * value - array of tags
	 * @var array
	 */
	private $beginTags = array();

	/**
	 * All end tags
	 * key - index in inputFile
	 * value - array of tags
	 * @var array
	 */
	private $endTags = array();

	/**
	 * Class constructor
	 * @param string $input Data from inputFile
	 * @param array $commands Data from formatFile
	 * @param boolean $isBr Parameter --br
	 */
	function __construct($input, $commands, $isBr)
	{
		$this->input = $input;
		$this->commands = $commands;
		$this->isBr = $isBr;
	}

	/**
	 * Function apply all rules from formatFile at data from inputFile
	 * Applying rules means, highlight text with html tags
	 * @param string $this->input Highlighted text
	 */
	public function highlightCode()
	{
		$match = array();

		// For every line in formatFile
		for ($i = 0; $i < count($this->commands); $i++)
		{
			$catchError = @preg_match_all('('.$this->commands[$i]['regex'].')s', $this->input, $match, PREG_OFFSET_CAPTURE);

			if ($catchError === false)
			{
				return FORMAT_FILE_ERROR;
			}

			// Loop each rule
			for ($k = 0; $k < count($match[0]); $k++)
			{
				$element = $match[0][$k];

				if (strlen($element[0]) == 0)
				{
					continue;
				}

				// first position of match
				$begin = $element[1];
				// last position of match
				$end = strlen($element[0]) + $begin;

				// Add tags to beginTags and endTags array
				for ($c = 0; $c < count($this->commands[$i]['commands']); $c++)
				{
					$tag = $this->commands[$i]['commands'][$c];

					if ($tag[0] == 'bold')
					{
						$this->beginTags[$begin] = array_key_exists($begin, $this->beginTags) ? $this->beginTags[$begin] . '<b>' : '<b>';
						$this->endTags[$end] = array_key_exists($end, $this->endTags) ? '</b>' . $this->endTags[$end] : '</b>';
					}
					elseif ($tag[0] == 'italic')
					{
						$this->beginTags[$begin] = array_key_exists($begin, $this->beginTags) ? $this->beginTags[$begin] . '<i>' : '<i>';
						$this->endTags[$end] = array_key_exists($end, $this->endTags) ? '</i>' . $this->endTags[$end] : '</i>';
					}
					elseif ($tag[0] == 'underline')
					{
						$this->beginTags[$begin] = array_key_exists($begin, $this->beginTags) ? $this->beginTags[$begin] .'<u>' : '<u>';
						$this->endTags[$end] = array_key_exists($end, $this->endTags) ? '</u>' . $this->endTags[$end]: '</u>';
					}
					elseif ($tag[0] == 'teletype')
					{
						$this->beginTags[$begin] = array_key_exists($begin, $this->beginTags) ? $this->beginTags[$begin] .'<tt>' : '<tt>';
						$this->endTags[$end] = array_key_exists($end, $this->endTags) ? '</tt>' . $this->endTags[$end]: '</tt>';
					}
					elseif ($tag[0] == 'size')
					{
						$this->beginTags[$begin] = array_key_exists($begin, $this->beginTags) ? $this->beginTags[$begin] . '<font size='. $tag[1] . '>': '<font size='. $tag[1] . '>';
						$this->endTags[$end] = array_key_exists($end, $this->endTags) ? '</font>' . $this->endTags[$end] : '</font>';
					}
					elseif ($tag[0] == 'color')
					{
						$this->beginTags[$begin] = array_key_exists($begin, $this->beginTags) ? $this->beginTags[$begin] . '<font color=#'. $tag[1] . '>' : '<font color=#'. $tag[1] . '>';
						$this->endTags[$end] = array_key_exists($end, $this->endTags) ? '</font>' . $this->endTags[$end] : '</font>';
					}
				}
			}
		}

		// Apply tags to input text
		if ($this->commands != null)
		{
			// Sort array by key from largest to smallest index
			krsort($this->beginTags);
			krsort($this->endTags);

			$inputDataSize = strlen($this->input);

			for ($i = $inputDataSize; $i > -1; $i--)
			{
				if (array_key_exists($i, $this->beginTags))
				{
					$this->input = substr_replace($this->input, $this->beginTags[$i], $i, 0);
				}

				if (array_key_exists($i, $this->endTags))
				{
					$this->input = substr_replace($this->input, $this->endTags[$i], $i, 0);
				}
			}
		}

		// Add <br /> before new line
		if ($this->isBr)
		{
			$brTags = array();

			preg_match_all('(\n)', $this->input, $match, PREG_OFFSET_CAPTURE);
			for ($i = 0; $i < count($match[0]); $i++)
			{
				$element = $match[0][$i];
				$brTags[$element[1]] = '<br />';
			}

			krsort($brTags);

			foreach ($brTags as $key => $value)
			{
				$this->input = substr_replace($this->input, $value, $key, 0);
			}
		}

		return $this->input;
	}
}

?>