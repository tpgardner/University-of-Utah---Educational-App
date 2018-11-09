-- phpMyAdmin SQL Dump
-- version 4.5.4.1deb2ubuntu2
-- http://www.phpmyadmin.net
--
-- Host: localhost
-- Generation Time: Dec 10, 2016 at 01:50 PM
-- Server version: 5.7.16-0ubuntu0.16.04.1
-- PHP Version: 7.0.8-0ubuntu0.16.04.3

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `ccc_app`
--

-- --------------------------------------------------------

--
-- Table structure for table `puzzles`
--

CREATE TABLE `puzzles` (
  `userid` int(11) NOT NULL,
  `puzzleid` int(11) NOT NULL,
  `date` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `puzzles`
--

INSERT INTO `puzzles` (`userid`, `puzzleid`, `date`) VALUES
(2, 1, '2016-11-30 07:28:08');

-- --------------------------------------------------------

--
-- Table structure for table `questions`
--

CREATE TABLE `questions` (
  `id` int(11) NOT NULL,
  `text` text NOT NULL,
  `correct_ans` text NOT NULL,
  `true_or_false` tinyint(1) NOT NULL,
  `mcq` tinyint(1) NOT NULL,
  `fill_in_the_blank` tinyint(1) NOT NULL,
  `drag_and_drop` tinyint(1) NOT NULL,
  `questionlevel` int(11) NOT NULL,
  `class_code` varchar(255) NOT NULL,
  `puzzle` text NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `users`
--

CREATE TABLE `users` (
  `id` int(11) NOT NULL,
  `username` varchar(255) NOT NULL,
  `password` varchar(255) NOT NULL,
  `dateofbirth` date NOT NULL,
  `teacher` tinyint(1) NOT NULL DEFAULT '0',
  `userlevel` int(11) NOT NULL DEFAULT '1',
  `class_code` varchar(255) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `users`
--

INSERT INTO `users` (`id`, `username`, `password`, `dateofbirth`, `teacher`, `userlevel`, `class_code`) VALUES
(1, 'meysam', 'test', '1995-07-30', 1, 1, 'test'),
(2, 'reza', '123123', '2016-12-31', 0, 1, 'test'),
(3, 'tom', '1234', '1987-12-22', 1, 1, '021'),
(4, 'sam123', '123', '1983-12-01', 0, 1, '021'),
(5, 'Reza Ali', '123', '2016-12-31', 1, 1, '011'),
(7, 'david', '123123', '1976-05-12', 1, 1, 'bestclass'),
(8, 'rose', '1234', '1976-04-12', 1, 1, 'theclass'),
(9, 'mary', '123123', '1987-12-31', 0, 1, 'theclass'),
(10, 'boss', '123', '1978-04-02', 1, 1, 'goodclass'),
(12, 'john', '1234', '1978-12-31', 1, 1, '012'),
(13, 'brian', '1234', '1988-02-14', 0, 1, '012'),
(14, 'Me', 'asas', '2000-12-12', 1, 1, 'qw12');

--
-- Indexes for dumped tables
--

--
-- Indexes for table `questions`
--
ALTER TABLE `questions`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `users`
--
ALTER TABLE `users`
  ADD PRIMARY KEY (`id`);

--
-- AUTO_INCREMENT for dumped tables
--

--
-- AUTO_INCREMENT for table `questions`
--
ALTER TABLE `questions`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;
--
-- AUTO_INCREMENT for table `users`
--
ALTER TABLE `users`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=15;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
