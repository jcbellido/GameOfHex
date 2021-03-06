BEGIN TRANSACTION;
CREATE TABLE `TranslationContentsHistory` (
                `TranslationContentHistoryId`    INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
                `Version`              INTEGER NOT NULL DEFAULT 0 CHECK(Version >=0),
                `Text`    TEXT,
                `TranslationContentId`  INTEGER,
                FOREIGN KEY(`TranslationContentId`) REFERENCES TranslationContents(TranslationContentId)
);
CREATE TABLE "TranslationContents" (
	`TranslationContentId`	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
	`Version`	INTEGER NOT NULL DEFAULT 0 CHECK(Version >= 0),
	`Text`	TEXT,
	`SourceLineContentId`	INTEGER,
	`LanguageId`	INTEGER,
	FOREIGN KEY(`SourceLineContentId`) REFERENCES `SourceLineContents`(`SourceLineContentId`),
	FOREIGN KEY(`LanguageId`) REFERENCES Languages(LanguageId)
);
CREATE TABLE "SourceLines" (
                `SourceLineId`   INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
                `StringId`              TEXT NOT NULL UNIQUE,
                `Platform`            INTEGER,
                `Status` INTEGER NOT NULL DEFAULT 0 CHECK(Status >= 0),
                FOREIGN KEY(`Platform`) REFERENCES `Platforms`(`PlatformId`)
);
CREATE TABLE "SourceLineContentsHistory" (
                `SourceLineContentHistoryId`    INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
                `Version`              INTEGER NOT NULL DEFAULT 0 CHECK(Version >= 0),
                `Text`    TEXT,
                `SourceLineContentId`   INTEGER,
                FOREIGN KEY(`SourceLineContentId`) REFERENCES `SourceLineContents`(`SourceLineContentId`)
);
CREATE TABLE "SourceLineContents" (
                `SourceLineContentId`   INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
                `Version`              INTEGER DEFAULT 0 CHECK(Version >= 0),
                `Text`    TEXT,
                `SourceLineId`   INTEGER,
                FOREIGN KEY(`SourceLineId`) REFERENCES `SourceLines`(`SourceLineId`)

);
CREATE TABLE `Platforms` (
                `PlatformId`        INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
                `Name` TEXT NOT NULL DEFAULT 'Windows' UNIQUE
);
CREATE TABLE `LineSets` (
	`LineSetId`	INTEGER NOT NULL UNIQUE,
	`Name`	TEXT NOT NULL,
	PRIMARY KEY(`LineSetId`)
);
CREATE TABLE `LineSetContents` (
	`LineSetId`	INTEGER NOT NULL,
	`SourceLineId`	INTEGER NOT NULL,
	FOREIGN KEY(`LineSetId`) REFERENCES LineSets(LinesetId),
	FOREIGN KEY(`SourceLineId`) REFERENCES SourceLines(SourceLineId)
);
CREATE TABLE `Languages` (
                `LanguageId`      INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
                `Name` TEXT NOT NULL DEFAULT 'English' UNIQUE,
                `IsSource`            INTEGER NOT NULL DEFAULT 0 CHECK(IsSource=0 OR IsSource=1)
);
CREATE INDEX `SourceLinesStringIdIndex` ON `SourceLines` (`StringId` );
COMMIT;
