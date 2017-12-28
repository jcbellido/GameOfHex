-- Create a handy view
CREATE VIEW vwCustomerOrderSummary WITH SCHEMABINDING AS
	SELECT 
		c.CustomerId, c.FirstName, c.LastName, r.RankName,
		ISNULL( SUM (oh.OrderTotal), 0) AS OrderTotal
	FROM
		dbo.Customer AS c
		LEFT OUTER JOIN dbo.OrderHeader AS oh ON c.CustomerId = oh.CustomerId
		LEFT OUTER JOIN dbo.Ranking AS r ON r.RankingId = c.RankingId
	GROUP BY
		c.CustomerId, c.FirstName, c.LastName, r.RankName

