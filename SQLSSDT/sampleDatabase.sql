--CREATE DATABASE SampleDb
--GO

USE SampleDb
GO

--CREATE TABLE Customer(
--	CustomerId bigint NOT NULL PRIMARY KEY,
--	FirstName varchar(50) NOT NULL,
--	LastName varchar(50) NOT NULL, 
--	CustomerRanking varchar(50) NULL )

--CREATE TABLE OrderHeader(
--	OrderHeaderId bigint NOT NULL,
--	CustomerId bigint NOT NULL,
--	OrderTotal money NOT NULL)

--ALTER TABLE OrderHeader ADD CONSTRAINT FK_OrderHeader_Customer
--FOREIGN KEY(CustomerId) REFERENCES Customer(CustomerId)

--INSERT INTO Customer (CustomerId, FirstName, LastName, CustomerRanking) VALUES
--(1, 'Lukas', 'Keller', NULL), 
--(2, 'Jeff', 'Hay', 'Good' ),
--(3, 'Keith', 'Harris', 'so-so' ),
--(4, 'Simon', 'Pearson', 'A+' ),
--(5, 'Matt', 'Hink', 'Stellar' ),
--(6, 'April', 'Reagan', '' )

-- INSERT INTO OrderHeader( OrderHeaderId, CustomerId, OrderTotal ) VALUES
-- (1, 2, 28.5 ), (2, 2, 169.0),
-- (3, 3, 12.99 ), 
-- (4, 4, 787.75), (5, 4, 250),
-- (6, 5, 6100), (7, 5, 4250),
-- (8, 6, 18.5), (9, 6, 10), (10, 6, 18)
-- GO

--CREATE VIEW vwCustomerOrderSummary WITH SCHEMABINDING AS
--	SELECT 
--		c.CustomerId, c.FirstName, c.LastName, c.CustomerRanking,
--		ISNULL( SUM (oh.OrderTotal), 0) AS OrderTotal
--	FROM
--		dbo.Customer AS c
--		LEFT OUTER JOIN dbo.OrderHeader AS oh ON c.CustomerId = oh.CustomerId
--	GROUP BY
--		c.CustomerId, c.FirstName, c.LastName, c.CustomerRanking
--	GO

SELECT * FROM Customer
SELECT * FROM vwCustomerOrderSummary

