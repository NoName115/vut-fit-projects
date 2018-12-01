<?php

define("OK", 0);
define("ARGUMENT_ERROR", 1);
define("INPUT_FILE_ERROR", 2);
define("OUTPUT_FILE_ERROR", 3);


class ArgumentParserAndFiles
{
	/**
	 * --help or -h parameter
	 * @var boolean
	 */
	private $help = false;

	/**
	 * --format=<file_path> or -f=<file_path> parameter
	 * @var boolean
	 */
	private $format = false;

	/**
	 * --input=<file_path> or -i=<file_path> parameter
	 * @var boolean
	 */
	private $input = false;

	/**
	 * --output=<file_path> -o=<file_path> parameter
	 * @var boolean
	 */
	private $output = false;

	/**
	 * --br parameter
	 * @var boolean
	 */
	private $br = false;

	/**
	 * Input file
	 * @var stream
	 */
	private $inputFile = null;

	/**
	 * Output file
	 * @var stream
	 */
	private $outputFile = null;

	/**
	 * Format file
	 * @var stream
	 */
	private $formatFile = null;

	/**
	 * Input program arguments
	 * @var array
	 */
	public $arguments = null;

	/**
	 * Class constructor
	 * @param array $argv Input program arguments
	 */
	function __construct($argv)
	{
		$this->arguments = $argv;
	}

	/**
	 * Function to parse input arguments
	 */
	function parseIt()
	{
		if (count($this->arguments) - 1 > 5)
		{
			return ARGUMENT_ERROR;
		}

		for ($i = 1; $i < count($this->arguments); $i++)
		{
			$actualArg = $this->arguments[$i];

			if ($actualArg == '-h' || $actualArg == '--help')
			{
				$this->help = true;

				if (count($this->arguments) == 2)
				{
					$this->printHelp();
					return 0;
				}
				else
				{
					return ARGUMENT_ERROR;
				}
			}
			elseif ($actualArg == '-b' || $actualArg == '--br')
			{
				if ($this->br)
				{
					return ARGUMENT_ERROR;
				}

				$this->br = true;
			}
			else
			{
				$splitArg = explode("=", $actualArg);

				if (count($splitArg) != 2)
				{
					return ARGUMENT_ERROR;
				}

				if ($splitArg[0] == '-f' || $splitArg[0] == '--format')
				{
					if ($this->format)
					{
						return ARGUMENT_ERROR;
					}

					$this->format = true;

					// Format file does not exist
					if (!realpath($splitArg[1]) || $splitArg[1] == '')
					{
						continue;
					}

					// Open file
					$this->formatFile = fopen(realpath($splitArg[1]), 'r');

					// File is empty
					if ($this->formatFile != null && filesize(realpath($splitArg[1])) == 0)
					{
						$this->formatFile = null;
						continue;
					}

					// Cant open file
					if ($this->formatFile == null)
					{
						continue;
					}
				}
				elseif ($splitArg[0] == '-i' || $splitArg[0] == '--input')
				{
					if ($this->input)
					{
						return ARGUMENT_ERROR;
					}

					if (!realpath($splitArg[1]))
					{
						return INPUT_FILE_ERROR;
					}

					$this->input = true;

					// Open file and convert path
					$this->inputFile = fopen(realpath($splitArg[1]), 'r');
					if ($this->inputFile == null)
					{
						return INPUT_FILE_ERROR;
					}
				}
				elseif ($splitArg[0] == '-o' || $splitArg[0] == '--output')
				{
					if ($this->output)
					{
						return ARGUMENT_ERROR;
					}

					$this->output = true;

					// Convert path
					if (realpath($splitArg[1]))
					{
						$splitArg[1] = realpath($splitArg[1]);
					}

					// Open file
					$this->outputFile = @fopen($splitArg[1], 'w');
					if ($this->outputFile == null)
					{
						return OUTPUT_FILE_ERROR;
					}
				}
				else
				{
					return ARGUMENT_ERROR;
				}
			}
		}

		// inputFile is stdin
		if ($this->inputFile == null)
		{
			$this->inputFile = fopen("php://stdin", 'r');
		}

		// outputFile is stdout
		if ($this->outputFile == null)
		{
			$this->outputFile = fopen("php://stdout", 'w');
		}

		return OK;
	}

	/**
	 * Function to print help info
	 */
	private function printHelp()
	{
		print("IPP - 1.Projekt_SYN\n");
		print("Author: xkolcu00\n");
		print("    --help               This help output\n");
		print("    --format=filename    Format file\n");
		print("    --input=filename     Input file\n");
		print("    --output=filename    Output file\n");
		print("    --br                 Add <br /> at the end of each line\n");
	}

	/**
	 * Function return if --br parameter is set
	 * @return boolean $this->br --br parameter
	 */
	public function getBr()
	{
		return $this->br;
	}

	/**
	 * Function read one line from formatFile
	 * @return string or boolean $line One line of formatFile
	 */
	public function getFormatLine()
	{
		// return false if is nothing to read
		if ($this->formatFile != null)
		{
			return $line = fgets($this->formatFile);
		}

		return false;
	}

	/**
	 * Read whole inputFile
	 * @return string $output Data from inputFile
	 */
	public function getInputFileData()
	{
		$output = '';
		while (($line = fgets($this->inputFile)) != false)
		{
			$output = $output . $line;
		}

		return $output;
	}

	/**
	 * Write input data to outputFile
	 * @param string $inputText Text that is written to outputFile
	 */
	public function printToOutputFile($inputText)
	{
		if ($this->outputFile != null)
		{
			fwrite($this->outputFile, $inputText);
		}
	}

	/**
	 * Function close inputFile
	 */
	public function closeInputFile()
	{
		if ($this->inputFile != null)
		{
			fclose($this->inputFile);
		}
	}

	/**
	 * Function close outputFile
	 */
	public function closeOutputFile()
	{
		if ($this->outputFile != null)
		{
			fclose($this->outputFile);
		}
	}

	/**
	 * Function close formatFile
	 */
	public function closeFormatFile()
	{
		if ($this->formatFile != null)
		{
			fclose($this->formatFile);
		}
	}

	/**
	 * Check and return if formatFile exist
	 * @return boolean If formatFile exist
	 */
	public function isFormatFile()
	{
		if (!$this->format || $this->formatFile == null)
		{
			return false;
		}

		return true;
	}

	/**
	 * Function return if --help/-h parameter is set
	 * @return boolean $this->help --help/-h parameter
	 */
	public function getHelp()
	{
		return $this->help;
	}
}

?>