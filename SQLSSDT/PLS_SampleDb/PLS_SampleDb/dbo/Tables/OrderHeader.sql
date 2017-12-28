CREATE TABLE [dbo].[OrderHeader] (
    [OrderHeaderId] BIGINT NOT NULL,
    [CustomerId]    BIGINT NOT NULL,
    [OrderTotal]    MONEY  NOT NULL,
    CONSTRAINT [FK_OrderHeader_Customer] FOREIGN KEY ([CustomerId]) REFERENCES [dbo].[Customer] ([CustomerId])
);

