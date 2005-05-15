/*
 * qca_cert.h - Qt Cryptographic Architecture
 * Copyright (C) 2003-2005  Justin Karneges <justin@affinix.com>
 * Copyright (C) 2004,2005  Brad Hards <bradh@frogmouth.net>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#ifndef QCA_CERT_H
#define QCA_CERT_H

#include <QMap>
#include <QDateTime>
#include "qca_core.h"
#include "qca_publickey.h"

namespace QCA
{
	class CertificateCollection;

	/**
	   Certificate Request Format
	*/
	enum CertificateRequestFormat
	{
		PKCS10, ///< standard PKCS#10 format
		SPKAC   ///< Signed Public Key and Challenge (Netscape) format
	};

	/**
	   Certificate information types
	*/
	enum CertificateInfoType
	{
		CommonName,     ///< The common name (eg person)
		Email,          ///< Email address
		Organization,   ///< An organisation (eg company)
		OrganizationalUnit,  ///< An part of an organisation (eg a division or branch)
		Locality,       ///< The locality (eg a shire, or part of a state) 
		State,          ///< The state within the country
		Country,        ///< The country
		URI,            
		DNS,
		IPAddress,
		XMPP
	};

	/**
	   Certificate constraints
	*/
	enum ConstraintType
	{
		// basic
		DigitalSignature,
		NonRepudiation,
		KeyEncipherment,
		DataEncipherment,
		KeyAgreement,
		KeyCertificateSign,
		CRLSign,
		EncipherOnly,
		DecipherOnly,

		// extended
		ServerAuth,
		ClientAuth,
		CodeSigning,
		EmailProtection,
		IPSecEndSystem,
		IPSecTunnel,
		IPSecUser,
		TimeStamping,
		OCSPSigning
	};

	/**
	   Specify the intended usage of a certificate
	*/
	enum UsageMode
	{
		UsageAny             = 0x00, ///< Any application, or unspecified
		UsageTLSServer       = 0x01, ///< server side of a TLS or SSL connection
		UsageTLSClient       = 0x02, ///< client side of a TLS or SSL connection
		UsageCodeSigning     = 0x04, ///< code signing certificate
		UsageEmailProtection = 0x08, ///< email (S/MIME) certificate
		UsageTimeStamping    = 0x10, ///< time stamping certificate
		UsageCRLSigning      = 0x20  ///< certificate revocation list signing certificate
	};

	/**
	   The validity (or otherwise) of a certificate
	*/
	enum Validity
	{
		ValidityGood,            ///< The certificate is valid
		ErrorRejected,           ///< The root CA rejected the certificate purpose
		ErrorUntrusted,          ///< The certificate is not trusted
		ErrorSignatureFailed,    ///< The signature does not match
		ErrorInvalidCA,          ///< The Certificate Authority is invalid
		ErrorInvalidPurpose,     ///< The purpose does not match the intended usage
		ErrorSelfSigned,         ///< The certificate is self-signed, and is not found in the list of trusted certificates
		ErrorRevoked,            ///< The certificate has been revoked
		ErrorPathLengthExceeded, ///< The path length from the root CA to this certificate is too long
		ErrorExpired,            ///< The certificate has expired
		ErrorExpiredCA,          ///< The Certificate Authority has expired
		ErrorValidityUnknown     ///< Validity is unknown
	};

	/**
	   Certificate properties type
	*/
	typedef QMultiMap<CertificateInfoType, QString> CertificateInfo;

	/**
	   %Certificate constraints type
	*/
	typedef QList<ConstraintType> Constraints;

	/**
	   \class CertificateOptions qca_cert.h QtCrypto

	   %Certificate options

	   \note In SPKAC mode, all options are ignored except for challenge
	*/
	class QCA_EXPORT CertificateOptions
	{
	public:
		/**
		   Create a Certificate options set

		   \param format the format to create the certificate request in
		*/
		CertificateOptions(CertificateRequestFormat format = PKCS10);

		/**
		   Standard copy constructor
		   
		   \param from the Certificate Options to copy into this object
		*/
		CertificateOptions(const CertificateOptions &from);
		~CertificateOptions();

		/**
		   Standard assignment operator
		   
		   \param from the Certificate Options to copy into this object
		*/
		CertificateOptions & operator=(const CertificateOptions &from);

		/**
		   test the format type for this certificate
		*/
		CertificateRequestFormat format() const;

		/**
		   Specify the format for this certificate

		   \param f the format to use
		*/
		void setFormat(CertificateRequestFormat f);

		/**
		   Test if the certificate options object is valid

		   \return true if the certificate options object is valid
		*/
		bool isValid() const;

		QString challenge() const;        // request
		CertificateInfo info() const;     // request or create

		/**
		   list the constraints on this certificate
		*/
		Constraints constraints() const;  // request or create

		/**
		   list the policies on this certificate
		*/
		QStringList policies() const;     // request or create

		/**
		   test if the certificate options include the certificate being a 
		   certificate authority
		  
		   \sa setAsCA
		*/
		bool isCA() const;                // request or create

		/**
		   return the path limit on this certificate
		*/
		int pathLimit() const;            // request or create

		/**
		   The serial number for the certificate
		*/
		QBigInteger serialNumber() const; // create

		/**
		   the first time the certificate will be valid
		*/
		QDateTime notValidBefore() const; // create
		
		/**
		   the last time the certificate is valid
		*/
		QDateTime notValidAfter() const;  // create

		void setChallenge(const QString &s);
		void setInfo(const CertificateInfo &info);

		/**
		   set the constraints on the certificate

		   \param constraints the constraints to be used for the certificate
		*/
		void setConstraints(const Constraints &constraints);

		/**
		   set the policies on the certificate

		   \param policies the policies to be used for the certificate
		*/
		void setPolicies(const QStringList &policies);

		/**
		   set the certificate options to include the certificate being
		   a certificate authority

		   \param pathLimit the number of intermediate certificates allowable
		*/
		void setAsCA(int pathLimit = 8); // value from Botan

		/**
		   Set the serial number property on this certificate

		   \param i the serial number to use
		*/
		void setSerialNumber(const QBigInteger &i);

		/**
		   Set the validity period for the certificate

		   \param start the first time this certificate becomes valid
		   \param end the last time this certificate is valid
		*/
		void setValidityPeriod(const QDateTime &start, const QDateTime &end);

	private:
		class Private;
		Private *d;
	};

	/**
	   \class Certificate qca_cert.h QtCrypto

	   Public Key (X.509) certificate

	   This class contains one X.509 certificate
	*/
	class QCA_EXPORT Certificate : public Algorithm
	{
	public:
		/**
		   Create an empty Certificate
		*/
		Certificate();

		/**
		   Create a Certificate from a PEM encoded file

		   \param fileName the name (and path, if required)
		   of the file that contains the PEM encoded certificate
		*/
		Certificate(const QString &fileName);

		/**
		   Create a Certificate with specified options and a specified private key

		   \param opts the options to use
		   \param key the private key for this certificate
		   \param provider the provider to use to create this key, if a particular provider is required
		*/
		Certificate(const CertificateOptions &opts, const PrivateKey &key, const QString &provider = QString());

		/**
		   Test if the certificate is empty (null)
		   \return true if the certificate is null
		*/
		bool isNull() const;

		/**
		   The earliest date that the certificate is valid
		*/
		QDateTime notValidBefore() const;

		/**
		   The latest date that the certificate is valid
		*/
		QDateTime notValidAfter() const;

		/**
		   Properties of the subject of the certificate
		*/
		CertificateInfo subjectInfo() const;

		/**
		   Properties of the issuer of the certificate
		*/
		CertificateInfo issuerInfo() const;

		/**
		   The constraints that apply to this certificate
		*/
		Constraints constraints() const;

		/**
		   The policies that apply to this certificate

		   Policies are specified as strings containing OIDs
		*/
		QStringList policies() const;

		/**
		   The common name of the subject of the certificate

		   Common names are normally the name of a person, company or organisation
		*/
		QString commonName() const;

		/**
		   The serial number of the certificate
		*/
		QBigInteger serialNumber() const;

		/**
		   The public key associated with the subject of the certificate
		*/
		PublicKey subjectPublicKey() const;

		/**
		   Test if the Certificate is valid as a Certificate Authority

		   \return true if the Certificate is valid as a Certificate Authority
		*/
		bool isCA() const;

		/**
		   Test if the Certificate is self-signed

		   \return true if the certificate is self-signed
		*/
		bool isSelfSigned() const;

		int pathLimit() const;

		/**
		   The signature on the certificate
		*/
		QSecureArray signature() const;

		/**
		   The signature algorithm used for the signature on this certificate
		*/
		SignatureAlgorithm signatureAlgorithm() const;

		/**
		   The key identifier associated with the subject
		*/
		QByteArray subjectKeyId() const;

		/**
		   The key identifier associated with the issuer
		*/
		QByteArray issuerKeyId() const;

		/**
		   Check the validity of a certificate

		   \param trusted a collection of trusted certificates
		   \param untrusted a collection of additional certificates, not necessarily trusted
		   \param u the use required for the certificate
		*/
		Validity validate(const CertificateCollection &trusted, const CertificateCollection &untrusted, UsageMode u = UsageAny) const;

		/**
		   Export the Certificate into a DER format
		*/
		QSecureArray toDER() const;

		/**
		   Export the Certificate into a PEM format
		*/
		QString toPEM() const;

		/**
		   Export the Certificate into PEM format in a file

		   \param fileName the name of the file to use
		*/
		bool toPEMFile(const QString &fileName) const;

		/**
		   Import the certificate from DER

		   \param a the array containing the certificate in DER format
		   \param result a pointer to a ConvertResult, which if not-null will be set to the conversion status
		   \param provider the provider to use, if a specific provider is required

		   \return the Certificate corresponding to the certificate in the provided array
		*/
		static Certificate fromDER(const QSecureArray &a, ConvertResult *result = 0, const QString &provider = QString());

		/**
		   Import the certificate from PEM format

		   \param s the string containing the certificate in PEM format
		   \param result a pointer to a ConvertResult, which if not-null will be set to the conversion status
		   \param provider the provider to use, if a specific provider is required

		   \return the Certificate corresponding to the certificate in the provided string
		*/
		static Certificate fromPEM(const QString &s, ConvertResult *result = 0, const QString &provider = QString());

		/**
		   Import the certificate from a file

		   \param fileName the name (and path, if required) of the file containing the certificate in PEM format
		   \param result a pointer to a ConvertResult, which if not-null will be set to the conversion status
		   \param provider the provider to use, if a specific provider is required

		   \return the Certificate corresponding to the certificate in the provided string
		*/
		static Certificate fromPEMFile(const QString &fileName, ConvertResult *result = 0, const QString &provider = QString());

		bool matchesHostname(const QString &host) const;

		/**
		   Test for equality of two certificates
		   
		   \return true if the two certificates are the same
		*/
		bool operator==(const Certificate &a) const;

		/**
		   Test for inequality of two certificates
		   
		   \return true if the two certificates are not the same
		*/
		bool operator!=(const Certificate &a) const;
	};
	
	/**
	   \class CertificateChain qca_cert.h QtCrypto

	   A chain of related Certificates

	   CertificateChain is a list (a QList) of certificates that are related by the 
	   signature from one to another. If Certificate C signs Certificate B, and Certificate
	   B signs Certificate A, then C, B and A form a chain.

	   The normal use of a CertificateChain is from a end-user Certificate (called
	   the primary, equivalent to QList::first()) though some intermediate Certificates
	   to some other Certificate (QList::last()), which might
	   be a root Certificate Authority, but does not need to be.

	   You can build up the chain using normal QList operations, such as QList::append()

	   \sa QCA::CertificateCollection for an alternative way to represent a group
	   of Certificates that do not necessarily have a chained relationship.
	*/
	class QCA_EXPORT CertificateChain : public QList<Certificate>
	{
	public:
		/**
		   Create an empty certificate chain
		*/
		CertificateChain();

		/**
		   Create a certificate chain, starting at the specified certificate

		   \param primary the end-user certificate that forms one end of the chain
		*/
		CertificateChain(const Certificate &primary);

		/**
		   Return the primary (end-user) Certificate
		*/
		const Certificate & primary() const;
	};

	/**
	   \class CertificateRequest qca_cert.h QtCrypto

	   Certificate Request

	   A Certificate Request is a unsigned request for a Certificate
	*/
	class QCA_EXPORT CertificateRequest : public Algorithm
	{
	public:
		/**
		   Create an empty certificate request
		*/
		CertificateRequest();

		/**
		   Create a certificate request based on the contents of a file

		   \param fileName the file (and path, if necessary) containing a PEM encoded certificate request
		*/
		CertificateRequest(const QString &fileName);

		/**
		   Create a certificate request based on specified options

		   \param opts the options to use in the certificate request
		   \param key the private key that matches the certificate being requested
		   \param provider the provider to use, if a specific provider is required
		*/
		CertificateRequest(const CertificateOptions &opts, const PrivateKey &key, const QString &provider = QString());
		/**
		   test if the certificate request is empty
		   
		   \return true if the certificate request is empty, otherwise false
		*/
		bool isNull() const;

		/**
		   Test if the certificate request can use a specified format

		   \param f the format to test for
		   \param provider the provider to use, if a specific provider is required
		   
		   \return true if the certificate request can use the specified format
		*/
		static bool canUseFormat(CertificateRequestFormat f, const QString &provider = QString());

		/**
		   the format that this Certificate request is in
		*/
		CertificateRequestFormat format() const;

		/**
		   Information on the subject of the certificate being requested

		   \note this only applies to PKCS#10 format certificate requests
		*/
		CertificateInfo subjectInfo() const; // PKCS#10 only

		/**
		   The constraints that apply to this certificate request

		   \note this only applies to PKCS#10 format certificate requests
		*/
		Constraints constraints() const;     // PKCS#10 only

		/**
		   The policies that apply to this certificate request

		   \note this only applies to PKCS#10 format certificate requests
		*/
		QStringList policies() const;        // PKCS#10 only

		/**
		   The public key belonging to the issuer
		*/
		PublicKey subjectPublicKey() const;

		/**
		   Test if this Certificate Request is for a Certificate Authority certificate

		   \note this only applies to PKCS#10 format certificate requests
		*/
		bool isCA() const;                   // PKCS#10 only

		/**
		   The path limit for the certificate in this Certificate Request

		   \note this only applies to PKCS#10 format certificate requests
		*/
		int pathLimit() const;               // PKCS#10 only

		/**
		   The challenge associated with this certificate request
		*/
		QString challenge() const;

		/**
		   The signature on this certificate request
		*/
		QSecureArray signature() const;

		/**
		   The algorithm used to make the signature on this certificate request
		*/
		SignatureAlgorithm signatureAlgorithm() const;

		/**
		   Export the Certificate Request into a DER format

		   \note this only applies to PKCS#10 format certificate requests
		*/
		QSecureArray toDER() const;

		/**
		   Export the Certificate Request into a PEM format

		   \note this only applies to PKCS#10 format certificate requests
		*/
		QString toPEM() const;

		/**
		   Export the Certificate into PEM format in a file

		   \param fileName the name of the file to use

		   \note this only applies to PKCS#10 format certificate requests
		*/
		bool toPEMFile(const QString &fileName) const;

		/**
		   Import the certificate request from DER

		   \param a the array containing the certificate request in DER format
		   \param result a pointer to a ConvertResult, which if not-null will be set to the conversion status
		   \param provider the provider to use, if a specific provider is required

		   \return the CertificateRequest corresponding to the certificate request in the provided array

		   \note this only applies to PKCS#10 format certificate requests
		*/
		static CertificateRequest fromDER(const QSecureArray &a, ConvertResult *result = 0, const QString &provider = QString());

		/**
		   Import the certificate request from PEM format

		   \param s the string containing the certificate request in PEM format
		   \param result a pointer to a ConvertResult, which if not-null will be set to the conversion status
		   \param provider the provider to use, if a specific provider is required

		   \return the CertificateRequest corresponding to the certificate request in the provided string

		   \note this only applies to PKCS#10 format certificate requests
		*/
		static CertificateRequest fromPEM(const QString &s, ConvertResult *result = 0, const QString &provider = QString());
		/**
		   Import the certificate request from a file

		   \param fileName the name (and path, if required) of the file containing the certificate request in PEM format
		   \param result a pointer to a ConvertResult, which if not-null will be set to the conversion status
		   \param provider the provider to use, if a specific provider is required

		   \return the CertificateRequest corresponding to the certificate request in the provided string

		   \note this only applies to PKCS#10 format certificate requests
		*/
		static CertificateRequest fromPEMFile(const QString &fileName, ConvertResult *result = 0, const QString &provider = QString());


		/**
		   Export the CertificateRequest to a string
		 
		   \return the string corresponding to the certificate request

		   \note this only applies to SPKAC format certificate requests
		*/
		QString toString() const;

		/**
		   Import the CertificateRequest from a string
		 
		   \param s the string containing to the certificate request
		   \param result a pointer to a ConvertResult, which if not-null will be set to the conversion status
		   \param provider the provider to use, if a specific provider is required

		   \return the CertificateRequest corresponding to the certificate request in the provided string

		   \note this only applies to SPKAC format certificate requests
		*/
		static CertificateRequest fromString(const QString &s, ConvertResult *result = 0, const QString &provider = QString());
	};

	/**
	   \class CRLEntry qca_cert.h QtCrypto

	   Part of a CRL representing a single certificate
	*/
	class QCA_EXPORT CRLEntry
	{
	public:
		/**
		   The reason why the certificate has been revoked
		*/
		enum Reason
		{
			Unspecified,        ///< reason is unknown
			KeyCompromise,      ///< private key has been compromised
			CACompromise,       ///< certificate authority has been compromised
			AffiliationChanged,
			Superceded,         ///< certificate has been superceded
			CessationOfOperation,
			CertificateHold,
			RemoveFromCRL,
			PrivilegeWithdrawn,
			AACompromise
		};

		/**
		   create an empty CRL entry
		*/
		CRLEntry();

		/**
		   create a CRL entry

		   \param c the certificate to revoke
		   \param r the reason that the certificate is being revoked
		*/
		CRLEntry(const Certificate &c, Reason r = Unspecified);

		/**
		   The serial number of the certificate that is the subject of this CRL entry
		*/
		QBigInteger serialNumber() const;

		/**
		   The time this CRL entry was created
		*/
		QDateTime time() const;

		/**
		   the reason that this CRL entry was created

		   Alternatively, you might like to think of this as the reason that the 
		   subject certificate has been revoked
		*/
		Reason reason() const;

	private:
		QBigInteger _serial;
		QDateTime _time;
		Reason _reason;
	};

	/**
	   \class CRL qca_cert.h QtCrypto

	   Certificate Revocation List
	*/
	class QCA_EXPORT CRL : public Algorithm
	{
	public:
		CRL();

		/**
		   Test if the CRL is empty

		   \return true if the CRL is entry, otherwise return false
		*/
		bool isNull() const;

		/**
		   Information on the issuer of the CRL
		*/
		CertificateInfo issuerInfo() const;

		/**
		   The CRL serial number
		*/
		int number() const;

		/**
		   the time that this CRL became (or becomes) valid
		*/
		QDateTime thisUpdate() const;

		/**
		   the time that this CRL will be obsoleted 

		   you should obtain an updated CRL at this time
		*/
		QDateTime nextUpdate() const;

		/** 
		    a list of the revoked certificates in this CRL
		*/
		QList<CRLEntry> revoked() const;

		/**
		   The signature on this CRL
		*/
		QSecureArray signature() const;

		/**
		   The signature algorithm used for the signature on this CRL
		*/
		SignatureAlgorithm signatureAlgorithm() const;

		/**
		   The key identification of the CRL issuer
		*/
		QByteArray issuerKeyId() const;

		/**
		   Export the Certificate Revocation List (CRL) in DER format

		   \return an array containing the CRL in DER format
		*/
		QSecureArray toDER() const;

		/**
		   Export the Certificate Revocation List (CRL) in PEM format

		   \return a string containing the CRL in PEM format
		*/
		QString toPEM() const;

		/**
		   Import a DER encoded Certificate Revocation List (CRL)

		   \param a the array containing the CRL in DER format
		   \param result a pointer to a ConvertResult, which if not-null will be set to the conversion status
		   \param provider the provider to use, if a specific provider is required

		   \return the CRL corresponding to the contents of the array
		*/
		static CRL fromDER(const QSecureArray &a, ConvertResult *result = 0, const QString &provider = QString());

		/**
		   Import a PEM encoded %Certificate Revocation List (CRL)

		   \param s the string containing the CRL in PEM format
		   \param result a pointer to a ConvertResult, which if not-null will be set to the conversion status
		   \param provider the provider to use, if a specific provider is required

		   \return the CRL corresponding to the contents of the string
		*/
		static CRL fromPEM(const QString &s, ConvertResult *result = 0, const QString &provider = QString());
	};

	/**
	   \class CertificateCollection qca_cert.h QtCrypto

	   Bundle of Certificates and CRLs

	   CertificateCollection provides a bundle of Certificates and Certificate Revocation
	   Lists (CRLs), not necessarily related.

	   \sa QCA::CertificateChain for a representation of a chain of Certificates related by
	   signatures.
	*/
	class CertificateCollection
	{
	public:
		/**
		   Create an empty Certificate / CRL collection
		*/
		CertificateCollection();

		/**
		   Standard copy constructor

		   \param from the CertificateCollection to copy from
		*/
		CertificateCollection(const CertificateCollection &from);

		~CertificateCollection();

		/**
		   Standard assignment operator

		   \param from the CertificateCollection to copy from
		*/
		CertificateCollection & operator=(const CertificateCollection &from);

		/**
		   Append a Certificate to this collection

		   \param cert the Certificate to add to this CertificateCollection
		*/
		void addCertificate(const Certificate &cert);

		/**
		   Append a CRL to this collection

		   \param crl the certificate revokation list to add to this CertificateCollection
		*/
		void addCRL(const CRL &crl);

		/**
		   The Certificates in this collection
		*/
		QList<Certificate> certificates() const;

		/**
		   The CRLs in this collection
		*/
		QList<CRL> crls() const;

		/**
		   Add another CertificateCollection to this collection

		   \param other the CertificateCollection to add to this collection
		*/
		void append(const CertificateCollection &other);

		/**
		   Add another CertificateCollection to this collection

		   \param other the CertificateCollection to add to this collection
		*/
		CertificateCollection operator+(const CertificateCollection &other) const;

		/**
		   Add another CertificateCollection to this collection

		   \param other the CertificateCollection to add to this collection
		*/
		CertificateCollection & operator+=(const CertificateCollection &other);

		// import / export

		/**
		   test if the CertificateCollection can be imported and exported to PKCS#7 format

		   \param provider the provider to use, if a specific provider is required

		   \return true if the CertificateCollection can be imported and exported to PKCS#7 format
		*/
		static bool canUsePKCS7(const QString &provider = QString());

		/**
		   export the CertificateCollection to a plain text file

		   \param fileName the name (and path, if required) to write the contents of the CertificateCollection to

		   \return true if the export succeeded, otherwise false
		*/
		bool toFlatTextFile(const QString &fileName);

		/**
		   export the CertificateCollection to a PKCS#7 file

		   \param fileName the name (and path, if required) to write the contents of the CertificateCollection to
		   \param provider the provider to use, if a specific provider is required

		   \return true if the export succeeded, otherwise false
		*/
		bool toPKCS7File(const QString &fileName, const QString &provider = QString());

		/**
		   import a CertificateCollection from a text file

		   \param fileName the name (and path, if required) to read the certificate collection from
		   \param result a pointer to a ConvertResult, which if not-null will be set to the conversion status
		   \param provider the provider to use, if a specific provider is required

		   \return the CertificateCollection corresponding to the contents of the file specified in fileName
		*/
		static CertificateCollection fromFlatTextFile(const QString &fileName, ConvertResult *result = 0, const QString &provider = QString());

		/**
		   import a CertificateCollection from a PKCS#7 file

		   \param fileName the name (and path, if required) to read the certificate collection from
		   \param result a pointer to a ConvertResult, which if not-null will be set to the conversion status
		   \param provider the provider to use, if a specific provider is required

		   \return the CertificateCollection corresponding to the contents of the file specified in fileName
		*/
		static CertificateCollection fromPKCS7File(const QString &fileName, ConvertResult *result = 0, const QString &provider = QString());

	private:
		class Private;
		QSharedDataPointer<Private> d;
	};

	/**
	   \class CertificateAuthority qca_cert.h QtCrypto

	   A %Certificate Authority is used to generate Certificates and
	   %Certificate Revocation Lists (CRLs).
	*/
	class QCA_EXPORT CertificateAuthority : public Algorithm
	{
	public:
		/**
		   Create a new %Certificate Authority

		   \param cert the CA certificate
		   \param key the private key associated with the CA certificate
		   \param provider the provider to use, if a specific provider is required
		*/
		CertificateAuthority(const Certificate &cert, const PrivateKey &key, const QString &provider);

		Certificate certificate() const;

		/**
		   Create a new Certificate by signing the provider CertificateRequest

		   \param req the CertificateRequest to sign
		   \param notValidAfter the last date that the Certificate will be valid
		*/
		Certificate signRequest(const CertificateRequest &req, const QDateTime &notValidAfter) const;

		/**
		   Create a new Certificate

		   \param key the Public Key to use to create the Certificate
		   \param opts the options to use for the new Certificate
		*/
		Certificate createCertificate(const PublicKey &key, const CertificateOptions &opts) const;

		/**
		   Create a new Certificate Revocation List (CRL)

		   \param nextUpdate the date that the CRL will be updated

		   \return an empty CRL
		*/
		CRL createCRL(const QDateTime &nextUpdate) const;

		/**
		   Update the CRL to include new entries

		   \param crl the CRL to update
		   \param entries the entries to add to the CRL
		   \param nextUpdate the date that this CRL will be updated

		   \return the update CRL
		*/
		CRL updateCRL(const CRL &crl, const QList<CRLEntry> &entries, const QDateTime &nextUpdate) const;
	};

	// holds a certificate chain and an associated private key
	class QCA_EXPORT KeyBundle
	{
	public:
		KeyBundle();
		KeyBundle(const QString &fileName, const QSecureArray &passphrase);
		KeyBundle(const KeyBundle &from);
		~KeyBundle();
		KeyBundle & operator=(const KeyBundle &from);

		bool isNull() const;

		QString name() const;
		CertificateChain certificateChain() const;
		PrivateKey privateKey() const;
		void setName(const QString &s);
		void setCertificateChainAndKey(const CertificateChain &c, const PrivateKey &key);

		// import / export
		QByteArray toArray(const QSecureArray &passphrase, const QString &provider = QString()) const;
		bool toFile(const QString &fileName, const QSecureArray &passphrase, const QString &provider = QString()) const;
		static KeyBundle fromArray(const QByteArray &a, const QSecureArray &passphrase, ConvertResult *result = 0, const QString &provider = QString());
		static KeyBundle fromFile(const QString &fileName, const QSecureArray &passphrase, ConvertResult *result = 0, const QString &provider = QString());

	private:
		class Private;
		QSharedDataPointer<Private> d;
	};

	// PGPKey can either reference an item in a real PGP keyring or can
	// be made by calling a "from" function.  Note that with the latter
	// method, the key is of no use besides being informational.  The
	// key must be in a keyring (inKeyring() == true) to actually do
	// crypto with it.
	class QCA_EXPORT PGPKey : public Algorithm
	{
	public:
		PGPKey();
		PGPKey(const QString &fileName);
		PGPKey(const PGPKey &from);
		~PGPKey();
		PGPKey & operator=(const PGPKey &from);

		bool isNull() const;

		QString keyId() const;
		QString primaryUserId() const;
		QStringList userIds() const;

		bool isSecret() const;
		QDateTime creationDate() const;
		QDateTime expirationDate() const;
		QString fingerprint() const;

		bool inKeyring() const;
		bool isTrusted() const;

		// import / export
		QSecureArray toArray() const;
		QString toString() const;
		bool toFile(const QString &fileName) const;
		static PGPKey fromArray(const QSecureArray &a, ConvertResult *result = 0, const QString &provider = QString());
		static PGPKey fromString(const QString &s, ConvertResult *result = 0, const QString &provider = QString());
		static PGPKey fromFile(const QString &fileName, ConvertResult *result = 0, const QString &provider = QString());
	};
}

#endif
