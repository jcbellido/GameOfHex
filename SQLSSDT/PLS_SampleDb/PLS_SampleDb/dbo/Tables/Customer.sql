CREATE TABLE [dbo].[Customer] (
    [CustomerId] BIGINT       NOT NULL,
    [FirstName]  VARCHAR (50) NOT NULL,
    [LastName]   VARCHAR (50) NOT NULL,
    RankingId INT NULL, 
    PRIMARY KEY CLUSTERED ([CustomerId] ASC), 
    CONSTRAINT [FK_Customer_CustomerRanking] FOREIGN KEY (RankingId) REFERENCES Ranking(RankingId)
);

