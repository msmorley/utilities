//---------------------------------------------------------------------------

#ifndef msmSerializerH
#define msmSerializerH
//---------------------------------------------------------------------------

#include <atomic>
#include <vector>
#include <set>

#include <boost/nowide/iostream.hpp>
#include <boost/iostreams/filtering_stream.hpp>

#include "msmLogger.h"

namespace msm
{

    using restoreStream= boost::iostreams::filtering_istream;
    using saveStream= boost::iostreams::filtering_ostream;

    // generic serialization class

	class serializer
	{
		protected:
			#ifdef _DEBUG
			std::size_t m_Offset;
			#endif

            std::size_t m_FileVersion= 0; // should be overridden in descendant constructors with their appropriate version numbers

		public:
			std::atomic_uint m_Progress;

			serializer(const std::size_t a_FileVersion= 0):
                m_FileVersion(a_FileVersion),
				#ifdef _DEBUG
				m_Offset(0),
				#endif
				m_Progress(0)
			{
			}

            virtual ~serializer()
            {
            }

            [[nodiscard]] const std::size_t __vectorcall fileVersion() const {return m_FileVersion;}

			#ifdef _DEBUG
			const std::size_t __vectorcall offset() {return m_Offset;}
            #endif
    };

	class restoreMatterStream: public serializer
	{
		private:
            using inherited= serializer;

			restoreStream& m_Stream;

        protected:
            const std::size_t __vectorcall readFileVersion()
            {
                m_Stream >> m_FileVersion;

                return m_FileVersion;
            }

		public:
			explicit restoreMatterStream(restoreStream& a_Stream):
     			inherited(),
				m_Stream(a_Stream)
			{
			}

            inline restoreStream& __vectorcall stream() {return m_Stream;}


            virtual const bool __vectorcall restore()
            {
                logLocation();
                readFileVersion();

                return true;
            }

			// stream operators

            restoreMatterStream& __vectorcall operator >>(std::string& value)
            {
                std::size_t length= 0;
                char* tempValue= nullptr;

                m_Stream.read(reinterpret_cast<char*>(&length), sizeof(length));

                #ifdef _DEBUG
                logLocationOffset(offset());
                m_Offset+= sizeof(length);
                #endif

                if (length > 0)
                {
                    tempValue= new char[length + 1];
                    memset(tempValue, 0, (length + 1) * sizeof(char));

                    m_Stream.read(reinterpret_cast<char*>(tempValue), length * sizeof(char));

                    #ifdef _DEBUG
                    m_Offset+= length * sizeof(char);
                    #endif

                    value= tempValue;

                    delete[] tempValue;
                }
                else
                {
                    value= "";
                }

                return *this;
            }

            template<class T>
           	restoreMatterStream& __vectorcall operator >>(T& value)
            {
                m_Stream.read(reinterpret_cast<char*>(&value), sizeof(T));

                #ifdef _DEBUG
				logLocationOffset(offset());
                m_Offset+= sizeof(T);
                #endif

                return *this;
            }

            template<class T, class U>
            restoreMatterStream& __vectorcall operator >> (std::pair<T,U> value)
            {
                logLocation();

                *this >> value.first;
                *this >> value.second;

				return *this;
            }

			template<class T>
			restoreMatterStream& __vectorcall operator >>(std::vector<T>& values)
			{
				std::size_t length= 0;

				m_Stream.read(reinterpret_cast<char*>(&length), sizeof(length));

                #ifdef _DEBUG
                logLocationOffset(offset());
                m_Offset+= sizeof(length);
                #endif

				values.clear();
				values.resize(length);

				for (auto& value : values)
				{
					*this >> value;
				}

				return *this;
			}

			template<class T>
			restoreMatterStream& __vectorcall operator >>(std::set<T>& values)
			{
				T value;

				std::size_t length;

				m_Stream.read(reinterpret_cast<char*>(&length), sizeof(length));

                #ifdef _DEBUG
                logLocationOffset(offset());
                m_Offset+= sizeof(length);
                #endif

				for (std::size_t loop= 0; loop < length; ++loop)
				{
					*this >> value;
					values.emplace(value);
				}

				return *this;
			}

	};


    class saveMatterStream: public serializer
    {
        private:
			using inherited= serializer;

            saveStream& m_Stream;

        protected:
			void __vectorcall writeFileVersion() const
            {
                m_Stream << m_FileVersion;
            }

		public:
			explicit saveMatterStream(saveStream& a_Stream, const std::size_t a_FileVersion):
                inherited(a_FileVersion),
                m_Stream(a_Stream)
            {
            }

            inline saveStream& __vectorcall stream() {return m_Stream;}

            virtual const bool __vectorcall save()
            {
                logLocation();
				// write version to stream
				writeFileVersion();

                return true;
            }

			// stream operators

			saveMatterStream& __vectorcall operator <<(std::string& value)
            {
                std::size_t length= 0;
                length= value.size();

                m_Stream.write(reinterpret_cast<char*>(&length), sizeof(length));

                #ifdef _DEBUG
                logLocationOffset(offset());
                m_Offset+= sizeof(length);
                #endif

                if (length > 0)
                {
                    m_Stream.write(reinterpret_cast<char*>(value.data()), length * sizeof(char));

                    #ifdef _DEBUG
                    m_Offset+= length * sizeof(char);
                    #endif
                }

                return *this;
            }

            template<class  T>
            saveMatterStream& __vectorcall operator <<(T value)
            {
                m_Stream.write(reinterpret_cast<char*>(&value), sizeof(value));

                #ifdef _DEBUG
                logLocationOffset(offset());
                m_Offset+= sizeof(value);
                #endif

                return *this;
            }

            template<class T>
            saveMatterStream& __vectorcall operator <<(std::vector<T>& values)
            {
                std::size_t length= values.size();
                m_Stream.write(reinterpret_cast<char*>(&length), sizeof(length));

                #ifdef _DEBUG
                logLocationOffset(offset());
                m_Offset+= sizeof(length);
                #endif

                for (auto& value : values)
                {
                    *this << value;
                }

                return *this;
            }

/*            template<class T>
            saveMatterStream& __vectorcall operator <<(std::vector<std::unique_ptr<T>>& values)
            {
                std::size_t length= values.size();
                m_Stream.write(reinterpret_cast<char*>(&length), sizeof(length));

                #ifdef _DEBUG
                logLocationOffset(offset());
                m_Offset+= sizeof(length);
                #endif

                for (auto& value : values)
                {
                    *this << value;
                }

                return *this;
            }
*/

            template<class T, class U>
            saveMatterStream& __vectorcall operator << (std::pair<T,U> value)
            {
                logLocation();

                *this << value.first;
                *this << value.second;
                return *this;
            }

			template<class T>
			saveMatterStream& __vectorcall operator <<(std::set<T>& values)
			{
				std::size_t length= values.size();

                #ifdef _DEBUG
                logLocationOffset(offset());
                m_Offset+= sizeof(length);
                #endif

				m_Stream.write(reinterpret_cast<char*>(&length), sizeof(length));

				for (auto& value : values)
				{
					*this << value;
				}

				return *this;
			}
	};

} // namespace msm

#endif
